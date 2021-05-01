/// Copyright (c) 2017 - present    Othmane AIT EL CADI <dartzon@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

/// \file      ObjFileParser.cpp
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#include "ObjFileParser.h"

#include "Utils.h"

#include <algorithm>

ObjDatabase ObjFileParser::parseFile()
{
    OBJLOG("Obj file parsing started...");

    namespace fs = std::filesystem;

    OBJASSERT((fs::exists(m_objFilePath) == true) && (fs::is_regular_file(m_objFilePath) == true),
              "Obj file not found");
    if ((fs::exists(m_objFilePath) == true) && (fs::is_regular_file(m_objFilePath) == true))
    {
        const std::unique_ptr<std::FILE, decltype(&fclose)> smtObjFile(fopen(m_objFilePath.c_str(),
                                                                             "r"),
                                                                       &fclose);

        if (smtObjFile != nullptr)
        {
            const uint16_t lineBufferSize = 1024;
            char lineBuffer[lineBufferSize];

            std::string oneLine;
            oneLine.reserve(lineBufferSize);

            // Create the default group named "default" before parsing the first entity.
            m_currentGroups.push_back(
                m_objDB.insertEntity(ObjEntityGroup{ElementType::GROUP_NAME, 0, "default"}));

            // Lambda that handles the case of multiple joind lines via the line continuation
            // character (\).
            auto handleLineContinuation = [&smtObjFile, &lineBuffer, &oneLine]() {
                for (size_t nextLinePos = oneLine.rfind('\\'); nextLinePos != std::string::npos;)
                {
                    oneLine[nextLinePos] = ' ';
                    if (fgets(lineBuffer, lineBufferSize, smtObjFile.get()) != nullptr)
                    {
                        oneLine += lineBuffer;
                    }

                    nextLinePos = oneLine.rfind('\\');
                }
            };

            while (fgets(lineBuffer, lineBufferSize, smtObjFile.get()) != nullptr)
            {
                oneLine = lineBuffer;

                handleLineContinuation();

                parseElement(oneLine);
            }

            // Set the last included entity index for any remaining active groups.
            endCurrentGroupsEntitiesRanges();

            OBJLOG("Obj file parsing ended");
        }
    }

    // std::move used because:
    // - Obj Database instance no longer needed by the Parser.
    // - Returning the instance as lvalue will call the copy ctor because m_objDB
    //   is an ObjFileParser member.
    return std::move(m_objDB);
}

// =================================================================================================

void ObjFileParser::parseElement(std::string_view oneLine)
{
    ObjUtils::StringUtils::removeSurroundingBlanks(oneLine);

    const std::optional<ElemIDResult_t> elemTypeRes = getElementType(oneLine);

    // False if the line is either empty or the element is unknown.
    if (elemTypeRes.has_value() == true)
    {
        constexpr std::array<ElementType, 4> arr = {ElementType::VERTEX,
                                                    ElementType::VERTEX_TEXTURE,
                                                    ElementType::VERTEX_NORMAL,
                                                    ElementType::VERTEX_PARAM_SPACE};

        const ElementType currentElemType = (*elemTypeRes).first;

        // Check if last element was a vertex (or its variants) and current element is not.
        if ((m_lastElementType != currentElemType) &&
            (std::find(arr.cbegin(), arr.cend(), m_lastElementType) != arr.cend()) &&
            (std::find(arr.cbegin(), arr.cend(), currentElemType) == arr.cend()))
        {
            // Pre-allocate memory for next wave of vertices indices.
            m_objDB.reserveIndexBufferMemory();
        }

        m_lastElementType = currentElemType;

        // Parse the arguments of each element.
        parseElementArgs(*elemTypeRes);
    }
}

// =================================================================================================

void ObjFileParser::parseElementArgs(const ElemIDResult_t& elementIDRes)
{
    const ElementType elemType = elementIDRes.first;

    switch (elemType)
    {
    case ElementType::VERTEX:
    case ElementType::VERTEX_TEXTURE:
    case ElementType::VERTEX_NORMAL:
    case ElementType::VERTEX_PARAM_SPACE: parseVertex(elementIDRes); break;

    case ElementType::FACE: parseFace(elementIDRes); break;

    case ElementType::GROUP_NAME:
    case ElementType::SMOOTHING_GROUP:
    case ElementType::MERGING_GROUP:
        // case ElementType::OBJECT_NAME:
        parseGroup(elementIDRes);
        break;

    case ElementType::MATERIAL_NAME: OBJLOG("usemtl command not yet supported"); break;

    case ElementType::LOD: OBJLOG("lod command not yet supported"); break;

    case ElementType::MATERIAL_LIB: OBJLOG("mtllib command not yet supported"); break;

    default:
        OBJLOG("Element's type not yet supported : ", static_cast<uint8_t>(elemType));
        OBJASSERT(false, "Element's type not yet supported");
        break;
    }
}

// =================================================================================================

std::optional<ElemIDResult_t> ObjFileParser::getElementType(std::string_view oneLine)
{
    // Skip this line if it is empty or is a comment line (\t, \f, \n, \r, \v,  ' ').
    if ((oneLine.size() > 0) && ((isspace(*oneLine.data()) != 0) || (*oneLine.data() == '#')))
    {
        return std::nullopt;
    }

    // Go to the next white space (Only \t & ' ').
    auto nextBlankItr = std::find_if(oneLine.cbegin(), oneLine.cend(), &isblank);

    // Extract the element's type keyword.
    // const size_t prefixToRemove = std::distance(oneLine.cbegin(), lineStartItr);
    const size_t suffixToRemove = std::distance(nextBlankItr, oneLine.cend());
    const std::string_view typeKeyword(oneLine.data(), oneLine.size() - suffixToRemove);

    // Find the element's type keyword in the keywords list.
    if (KeywordsMap_t::const_iterator keyItr = elementsKeywords.find(typeKeyword);
        keyItr == elementsKeywords.cend())
    {
        return std::nullopt;
    }
    else
    {
        // Keep only the element's arguments.
        oneLine.remove_prefix(std::distance(oneLine.cbegin(), nextBlankItr));
        ObjUtils::StringUtils::removeSurroundingBlanks(oneLine);

        // Store the element's type and its arguments.
        return std::make_pair(keyItr->second, oneLine);
    }
}

// =================================================================================================

std::optional<VerticesIdxOrganization>
ObjFileParser::getIndicesOrganization(const std::string_view& args)
{
    // Looks like: x.
    // Initialize to no slashes found, geometric vertex index only.
    VerticesIdxOrganization vtxIdxOrg = VerticesIdxOrganization::VGEO;

    if (const size_t doubleSlashesPos = args.find("//"); doubleSlashesPos != std::string_view::npos)
    {
        // Looks like: v//vn.
        // 2 contiguous slashes found, geometric vertex index and vertex normal index available.
        vtxIdxOrg = VerticesIdxOrganization::VGEO_VNORMAL;
    }
    else
    {
        auto firstBlankItr = std::find_if(args.cbegin(), args.cend(), &isblank);

        switch (const size_t countSlashes = std::count(args.cbegin(), firstBlankItr, '/');
                countSlashes)
        {
        case 0:
            // Looks like: v.
            // No slashes found, only geometric vertex available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO;
            break;

        case 1:
            // Looks like: v/vt.
            // 1 slash found, geometric vertex index and vertex texture index available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO_VTEXTURE;
            break;

        case 2:
            // Looks like: v/vt/vn.
            // 2 slashes found, all indices available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL;
            break;

        default: OBJASSERT(false, "Too many indices references"); return std::nullopt;
        }
    }

    return vtxIdxOrg;
}

// =================================================================================================

void ObjFileParser::parseVertex(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing a Vertex");

    auto [vtxType, vtxArgs] = elementIDRes;

    size_t processedCharsCount = 0;

    Vertex_t vtx{vtxType};
    auto& [x, y, z, w] = vtx;

    // Parse x component.
    x = std::stof(vtxArgs.data(), &processedCharsCount);
    vtxArgs.remove_prefix(processedCharsCount);

    // Parse y component.
    y = std::stof(vtxArgs.data(), &processedCharsCount);
    vtxArgs.remove_prefix(processedCharsCount);

    // Check if the z component exists.
    if (vtxArgs.size() > 0)
    {
        z = std::stof(vtxArgs.data(), &processedCharsCount);
        vtxArgs.remove_prefix(processedCharsCount);
    }

    // Check if the w component exists.
    if (vtxArgs.size() > 0)
    {
        w = std::stof(vtxArgs.data(), &processedCharsCount);
    }

    m_objDB.insertEntity(vtx);
}

// =================================================================================================

void ObjFileParser::parseFace(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing a Face");

    std::string_view faceArgs{elementIDRes.second};

    // Get the organization of the vertices' indices triplets.
    const std::optional<VerticesIdxOrganization> vtxIdxOrg = getIndicesOrganization(faceArgs);

    if (vtxIdxOrg.has_value() == false)
    {
        return;
    }

    const std::vector<std::string_view> parts = ObjUtils::StringUtils::splitString(faceArgs, {'/'});
    if (parts.size() < 3)
    {
        return;
    }

    const size_t indexBufferOldSize = m_objDB.getIndexBufferCount();
    for (const std::string_view& part : parts)
    {
        const int64_t vtxIdx = std::stol(part.data());
        m_objDB.insertIndex(vtxIdx);
    }
    const size_t indexBufferNewSize = m_objDB.getIndexBufferCount();

    m_objDB.insertEntity(ObjEntityFace(indexBufferOldSize, indexBufferNewSize - 1, *vtxIdxOrg));
}

// =================================================================================================

void ObjFileParser::parseGroup(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing a Group");

    // Set the last included entity index for all the previous active groups
    // before parsing new ones.
    endCurrentGroupsEntitiesRanges();

    // Clear the list of previous active groups in order to receive the new ones.
    m_currentGroups.clear();

    auto [grpType, grpArgs] = elementIDRes;

    switch (const size_t entityTableIdx = m_objDB.getEntitiesCount(); grpType)
    {
    case ElementType::GROUP_NAME:
    {
        const std::vector<std::string_view> grpNames = ObjUtils::StringUtils::splitString(grpArgs);
        for (const std::string_view& grpName : grpNames)
        {
            m_currentGroups.push_back(
                m_objDB.insertEntity(ObjEntityGroup{grpType, entityTableIdx, grpName}));
        }
    }
    break;

    case ElementType::OBJECT_NAME:
    {
        m_objDB.insertEntity(ObjEntityGroup{grpType, entityTableIdx, grpArgs});
    }
    break;

    case ElementType::SMOOTHING_GROUP:
    {
        if (grpArgs != "off" && grpArgs != "0")
        {
            const size_t groupNum = std::stol(grpArgs.data());

            m_currentGroups.push_back(
                m_objDB.insertEntity(ObjEntityGroup{grpType, entityTableIdx, groupNum}));
        }
    }
    break;

    case ElementType::MERGING_GROUP:
    {
        if (grpArgs != "off" && grpArgs != "0")
        {
            const std::vector<std::string_view> grpNbrAndRes = ObjUtils::StringUtils::splitString(
                grpArgs);

            const size_t groupNum = std::stol(grpNbrAndRes[0].data());

            uint32_t resolution = 0;
            if (grpNbrAndRes.size() > 1)
            {
                resolution = std::stol(grpNbrAndRes[1].data());
            }

            m_currentGroups.push_back(m_objDB.insertEntity(
                ObjEntityGroup{grpType, entityTableIdx, groupNum, resolution}));
        }
    }
    break;

    default: OBJASSERT(false, "Unknown groupe type");
    };
}

// =================================================================================================

void ObjFileParser::endCurrentGroupsEntitiesRanges()
{
    for (size_t grpIdx : m_currentGroups)
    {
        std::optional<std::reference_wrapper<ObjEntityGroup>> grpOpt = m_objDB.getGroup(grpIdx);
        OBJASSERT(grpOpt.has_value() == true, "Invalid group index");

        ObjEntityGroup& grp = *grpOpt;
        grp.endIncludedEntityRange(m_objDB.getEntitiesCount() - 1);
    }
}
