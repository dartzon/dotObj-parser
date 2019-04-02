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

    OBJASSERT((fs::exists(m_objFilePath) == true) && (fs::is_regular_file(m_objFilePath) == true),
              "Obj file not found");

    namespace fs = std::filesystem;
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
                parseLine(oneLine);
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

void ObjFileParser::parseLine(const std::string& oneLine)
{
    const std::optional<ElemIDResult_t> elemTypeRes = getElementType(oneLine);

    // The line is either empty or the element is unknown.
    if (elemTypeRes.has_value() == true)
    {
        // Check if last element was a vertex (or its variants) and current element is not.
        constexpr std::array<ElementType, 4> arr = {ElementType::VERTEX,
                                                    ElementType::VERTEX_TEXTURE,
                                                    ElementType::VERTEX_NORMAL,
                                                    ElementType::VERTEX_PARAM_SPACE};

        if ((m_lastElementType != (*elemTypeRes).first) &&
            (std::find(arr.cbegin(), arr.cend(), m_lastElementType) != arr.cend()) &&
            (std::find(arr.cbegin(), arr.cend(), (*elemTypeRes).first) == arr.cend()))
        {
            // Pre-allocate memory for next wave of vertices indices.
            m_objDB.reserveIndexBufferMemory();
        }

        m_lastElementType = (*elemTypeRes).first;

        // Parse the arguments of each element.
        parseElementArgs(*elemTypeRes);
    }
}

// =================================================================================================

void ObjFileParser::parseElementArgs(const ElemIDResult_t& elementIDRes)
{
    switch (elementIDRes.first)
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

    default: break;
    }
}

// =================================================================================================

std::optional<ElemIDResult_t> ObjFileParser::getElementType(const std::string& oneLine)
{
    // Skip white spaces at the beginning of the line (Only \t & ' ').
    CStringIterator_t lineStartItr = std::find_if_not(oneLine.cbegin(), oneLine.cend(), &isblank);

    // Skip this line if it is empty or is a comment line (\t, \f, \n, \r, \v,  ' ').
    if ((lineStartItr != oneLine.cend()) &&
        ((isspace(*lineStartItr) != 0) ||
         (*lineStartItr == '#')))  // TODO: check the first condition.
    {
        return std::nullopt;
    }

    // Go to the next white space (Only \t & ' ').
    CStringIterator_t nextBlankItr = std::find_if(lineStartItr, oneLine.cend(), &isblank);

    // Extract the element's type keyword.
    std::string_view elementToken(oneLine);
    elementToken.remove_prefix(std::distance(oneLine.cbegin(), lineStartItr));
    elementToken.remove_suffix(std::distance(nextBlankItr, oneLine.cend()));

    // Find the element's type keyword in the keywords list.
    if (KeywordsMap_t::const_iterator keyItr = elementsKeywords.find(elementToken);
        keyItr == elementsKeywords.cend())
    {
        return std::nullopt;
    }
    else
    {
        // Store the element's type and its arguments.
        elementToken = oneLine;
        elementToken.remove_prefix(std::distance(oneLine.cbegin(), nextBlankItr));
        return std::make_pair(keyItr->second, elementToken);
    }
}

// =================================================================================================

VerticesIdxOrganization ObjFileParser::getIndicesOrganization(const std::string& args)
{
    // Initialize to no slashes found. So, no texture vertices, no normal vertices.
    VerticesIdxOrganization vtxIdxOrg = VerticesIdxOrganization::VGEO;

    const size_t doubleSlashesPos = args.find("//");
    if (doubleSlashesPos != std::string::npos)
    {
        // 2 contiguous slashes found, geometric vertex index and vertex normal index available.
        vtxIdxOrg = VerticesIdxOrganization::VGEO_VNORMAL;
    }
    else
    {
        CStringIterator_t firstBlankItr = std::find_if(args.cbegin(), args.cend(), &isblank);
        const size_t countSlashes = std::count(args.cbegin(), firstBlankItr, '/');

        switch (countSlashes)
        {
        case 1:
            // 1 slash found, geometric vertex index and vertex texture index available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO_VTEXTURE;
            break;

        case 2:
            // 2 separate slashes found, all indices available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL;
            break;

        default: OBJASSERT(false, "Too many indices references");
        }
    }

    return vtxIdxOrg;
}

// =================================================================================================

void ObjFileParser::parseVertex(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing Vertex");

    auto [vtxType, vtxArgs] = elementIDRes;
    ObjUtils::StringUtils::removeSurroundingBlanks(vtxArgs);

    size_t processedCharsCount = 0;

    Vertex_t vtx;

    // Parse x component.
    vtx.m_x = std::stof(vtxArgs.data(), &processedCharsCount);
    vtxArgs.remove_prefix(processedCharsCount);

    // Parse y component.
    vtx.m_y = std::stof(vtxArgs.data(), &processedCharsCount);
    vtxArgs.remove_prefix(processedCharsCount);

    // Check if the z component exists.
    if (vtxArgs.size() > 0)
    {
        vtx.m_z = std::stof(vtxArgs.data(), &processedCharsCount);
    }

    m_objDB.insertVertex(vtx, vtxType);
}

// =================================================================================================

void ObjFileParser::parseFace(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing Face");

    std::string args(elementIDRes.second.data());
    ObjUtils::StringUtils::removeSurroundingBlanks(args);

    // Get the organization of the vertices' indices triplets.
    const VerticesIdxOrganization vtxIdxOrg = getIndicesOrganization(args);

    // Remove all the slashes and keep only numbers.
    std::replace(args.begin(), args.end(), '/', ' ');

    size_t idxCount = 0;
    size_t pos = 0;

    const std::array<size_t, 3> buffersPtrs = {m_objDB.getIndexBufferCount(),
                                               m_objDB.getTextureUVIndexBufferCount(),
                                               m_objDB.getVertexNormalIndexBufferCount()};
    uint8_t bufferPtrIdx = 0;

    while (pos < args.size())
    {
        args.erase(args.cbegin(), args.cbegin() + pos);
        int64_t vtxIdx = std::stol(args, &pos);

        // Negative indices refere to the last nth position in a buffer = buffer.size - idx.
        if (vtxIdx < 0)
        {
            switch (vtxIdxOrg)
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

        m_objDB.insertIndex(vtxIdx);
        ++idxCount;
    }

    OBJASSERT(idxCount <= 12, "Face has too many vertices indices");

    ObjEntityFace objFace(m_objDB.getIndexBufferCount() - idxCount, idxCount, vtxIdxOrg);
    m_objDB.insertEntity(objFace, m_currentGroupsIdx);
}

// =================================================================================================

void ObjFileParser::parseGroup(const ElemIDResult_t& elementIDRes)
{
    OBJLOG("Parsing Group");

    std::string args(elementIDRes.second.data());
    ObjUtils::StringUtils::removeSurroundingBlanks(args);

    if (elementIDRes.first == ElementType::GROUP_NAME ||
        elementIDRes.first == ElementType::OBJECT_NAME)
    {
        // Only a group name points to an entity index.
        const size_t entityTableIdx = (elementIDRes.first == ElementType::GROUP_NAME) ?
                                          m_objDB.getEntitiesCount() :
                                          0;
        ObjGroup grp(args, entityTableIdx, elementIDRes.first);
        m_objDB.insertGroup(grp);

        // Store this Obj group's index in the current Groups buffer.
        (elementIDRes.first == ElementType::GROUP_NAME) ?
            m_currentGroupsIdx[0] = m_objDB.getGroupsCount() - 1 :
            m_currentGroupsIdx[1] = m_objDB.getGroupsCount() - 1;
    }
    else if (elementIDRes.first == ElementType::SMOOTHING_GROUP)
    {
        const size_t groupNum = std::stol(args);

        ObjGroup grp(groupNum, m_objDB.getEntitiesCount(), 0, elementIDRes.first);
        m_objDB.insertGroup(grp);

        // Store this Obj group's index in the current Groups buffer.
        m_currentGroupsIdx[2] = m_objDB.getGroupsCount() - 1;
    }
}
