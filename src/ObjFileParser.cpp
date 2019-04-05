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
            const uint16_t lineBufferSize = 256;
            char lineBuffer[lineBufferSize];

            std::string oneLine;
            oneLine.reserve(lineBufferSize);

            while (fgets(lineBuffer, lineBufferSize, smtObjFile.get()) != nullptr)
            {
                oneLine = lineBuffer;
                parseElement(oneLine);
            }

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

    default: /* OBJASSERT(false, "Element's type not yet supported"); */
        OBJLOG("Element's type not yet supported : ", static_cast<uint8_t>(elemType));
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

    Vertex_t vtx;
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

    m_objDB.insertVertex(vtx, vtxType);
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

    size_t idxCount = 0;
    size_t pos = 0;

    const std::array<size_t, 3> buffersPtrs = {m_objDB.getIndexBufferCount(),
                                               m_objDB.getTextureUVIndexBufferCount(),
                                               m_objDB.getVertexNormalIndexBufferCount()};
    uint8_t bufferPtrIdx = 0;

    while (pos < faceArgs.size())
    {
        int64_t vtxIdx = std::stol(faceArgs.data(), &pos);

        // Negative indices refere to the last nth position in a buffer = buffer.size - idx.
        if (vtxIdx < 0)
        {
            switch (*vtxIdxOrg)
            {
            case VerticesIdxOrganization::VGEO_VTEXTURE:
                bufferPtrIdx = (bufferPtrIdx == 0) ? 1 : 0;
                break;

            case VerticesIdxOrganization::VGEO_VNORMAL:
                bufferPtrIdx = (bufferPtrIdx == 0) ? 2 : 0;
                break;

            case VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL:
                bufferPtrIdx = (bufferPtrIdx < 2) ? ++bufferPtrIdx : 0;
                break;

            default: break;
            }

            vtxIdx = buffersPtrs[bufferPtrIdx] + vtxIdx + 1;
        }

        // Skip slashes and already converted values.
        {
            uint8_t charsToRemove = 0;

            if (faceArgs[pos] == '/')
            {
                charsToRemove = 1;
                if (faceArgs[pos + 1] == '/')
                {
                    charsToRemove = 2;
                }
            }

            faceArgs.remove_prefix(pos + charsToRemove);
        }

        m_objDB.insertIndex(vtxIdx);
        ++idxCount;
    }

    OBJASSERT(idxCount <= 12, "Face has too many vertices indices");

    ObjEntityFace objFace(m_objDB.getIndexBufferCount() - idxCount, idxCount, *vtxIdxOrg);
    m_objDB.insertEntity(objFace, m_currentGroupsIdx);
}

// =================================================================================================

void ObjFileParser::parseGroup(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing a Group");

    auto [grpType, grpArgs] = elementIDRes;

    // std::vector<std::string_view> vec = ObjUtils::StringUtils::splitString(grpArgs);

    if (grpType == ElementType::GROUP_NAME || grpType == ElementType::OBJECT_NAME)
    {
        // Only a group name points to an entity index.
        const size_t entityTableIdx = (grpType == ElementType::GROUP_NAME) ?
                                          m_objDB.getEntitiesCount() :
                                          0;
        ObjGroup grp(grpArgs, entityTableIdx, grpType);
        m_objDB.insertGroup(grp);

        // Store this Obj group's index in the current Groups buffer.
        (grpType == ElementType::GROUP_NAME) ?
            m_currentGroupsIdx[0] = m_objDB.getGroupsCount() - 1 :
            m_currentGroupsIdx[1] = m_objDB.getGroupsCount() - 1;
    }
    else if ((grpType == ElementType::SMOOTHING_GROUP) && (grpArgs != "off" && grpArgs != "0"))
    {
        const size_t groupNum = std::stol(grpArgs.data());

        ObjGroup grp(groupNum, m_objDB.getEntitiesCount(), 0, grpType);
        m_objDB.insertGroup(grp);

        // Store this Obj group's index in the current Groups buffer.
        m_currentGroupsIdx[2] = m_objDB.getGroupsCount() - 1;
    }
}
