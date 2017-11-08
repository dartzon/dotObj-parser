// =============================================================================
// Copyright (c) 2017 Othmane AIT EL CADI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

/*
 * \file      main.cpp
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      04-11-2017
 */

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>
#include "Utils.h"
#include "Types.h"
#include "ObjGroup.h"
#include "ObjEntity.h"
#include "WithVerticesIndices.h"

// ------------------------------------------------------------------
//   GLOBALS
// ------------------------------------------------------------------
VertexBuffer_t gVBuffer;
TextureUVBuffer_t gTexUVBuffer;
NormalBuffer_t gNormalBuffer;
IndexBuffer_t gIdxBuffer;

constexpr KeywordsArray_t elementsKeywords = {"v", "vt", "vn", "vp",
                                              "deg", "bmat", "step", "cstype",
                                              "p", "l", "f", "curv", "curv2", "surf",
                                              "parm", "trim", "hole", "scrv", "sp", "end",
                                              "con",
                                              "g", "s", "mg", "o",
                                              "bevel", "c_interp", "d_interp", "lod",
                                              "usemtl", "mtllib", "shadow_obj", "trace_obj",
                                              "ctech", "stech"};

// ------------------------------------------------------------------
//   CLASSES
// ------------------------------------------------------------------

class ObjEntityFace : public ObjEntity, public WithVerticesIndices
{
public:
    ObjEntityFace(const size_t indexBufferIdx, const size_t indicesOffset,
                  const VerticesIdxOrganization eParamsOrganization) :
        ObjEntity(ElementType::FACE),
        WithVerticesIndices(indexBufferIdx, indicesOffset, eParamsOrganization)
    {
        // Determine if this face is a triangle.
        switch(eParamsOrganization)
        {
        case VerticesIdxOrganization::VGEO:
            m_isTriangle = (indicesOffset == 3);
            break;

        case VerticesIdxOrganization::VGEO_VTEXTURE:
        case VerticesIdxOrganization::VGEO_VNORMAL:
            m_isTriangle = (indicesOffset == 6);
            break;

        case VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL:
            m_isTriangle = (indicesOffset == 9);
            break;

        default: m_isTriangle = false;
        };
    }

    bool isTriangle(void) const
    {
        return (m_isTriangle);
    }

    ObjEntityFace(const ObjEntityFace&) = default;
    ObjEntityFace(ObjEntityFace&&) = default;

private:
    bool m_isTriangle;
};

class ObjDatabase
{
public:
    void insertEntity(ObjEntity& obj)
    {
        switch(obj.getType())
        {
        case ElementType::FACE:
            obj.setID(m_facesTable.size() + 1);
            m_facesTable.push_back(static_cast<ObjEntityFace&&>(obj));
            break;

        default: break;
        };

        std::for_each(m_currentGroupsIdx.cbegin(), m_currentGroupsIdx.cend(),
                      [this](const int64_t idx)
                      {
                          if(idx >= 0)
                          {
                              ++m_groupTable[idx];
                          }
                      });

        ++m_totalEntitiesCount;
    }
    ObjEntity* getNextEntity(void) const
    {
        ObjEntity* pObjEnt = nullptr;

        if(m_entityTable.empty() == false)
        {
            pObjEnt = m_entityTable.front();
            m_entityTable.pop();
        }

        return (pObjEnt);
    }
    size_t getEntitiesCount(void) const
    {
        return (m_totalEntitiesCount);
    }

    ObjGroup getNextGroup(void) const
    {
        const ObjGroup grp = std::move(m_groupTable.front());
        m_groupTable.erase(m_groupTable.cbegin());

        return (grp);
    }
    void insertGroup(ObjGroup&& grp)
    {
        switch(grp.getType())
        {
        case ElementType::GROUP:
        m_currentGroupsIdx[0] = m_groupTable.size();
        break;

        case ElementType::SMOOTHING_GROUP:
        m_currentGroupsIdx[1] = m_groupTable.size();
        break;

        case ElementType::MERGING_GROUP:
        m_currentGroupsIdx[2] = m_groupTable.size();
        break;

        default: break;
        }

        m_groupTable.push_back(std::move(grp));
    }
    size_t getGroupsCount(void) const
    {
        return (m_groupTable.size());
    }

    void sync(void)
    {
        if(m_DBSynced == false)
        {
            for(ObjEntityFace& face : m_facesTable)
            {
                m_entityTable.push(&face);
            }

            m_DBSynced = true;
        }
    }

private:
    mutable std::queue<ObjEntity*> m_entityTable;         //< Queue of pointers to all the entities.
    std::vector<ObjEntityFace> m_facesTable;                   //< Vector of Face entities.
    mutable std::vector<ObjGroup> m_groupTable;                   //< Queue of groups.
    std::array<int64_t, 3> m_currentGroupsIdx = {-1, -1, -1};  //< Vector of current groups.
    size_t m_totalEntitiesCount = 0;                           //< Total number of entities.
    bool m_DBSynced = false;                                   //< Was the entityTable synced?
};

// ------------------------------------------------------------------
//   GLOBALS 2
// ------------------------------------------------------------------
DocumentStats gDocStats;

ObjDatabase gObjDB;

// ------------------------------------------------------------------

// ------------------------------------------------------------------
//   UTILS
// ------------------------------------------------------------------

void removeTrailingBlanks(std::string& str)
{
    // Look for the first non blank character starting from the end of the string.
    RStringIterator_t strRItr = std::find_if_not(str.rbegin(), str.rend(), [](const char c)
                                               {
                                                   return (isspace(c) != 0);
                                               });

    if(strRItr != str.rend())
    {
        // Resize the string to remove blanks.
        const size_t sz = std::distance(str.rbegin(), strRItr);
        str.resize(str.size() - sz);
    }
}

void removeSurroundingBlanks(std::string& str)
{
    // Look for the first non blank character.
    CStringIterator_t strItr = std::find_if(str.cbegin(), str.cend(), [](const char c)
                                            {
                                                return (isblank(c) == 0);
                                            });
    if(strItr != str.end())
    {
        str.erase(str.cbegin(), strItr);
    }

    // Look for the first non blank character starting from the end of the string.
    RStringIterator_t strRItr = std::find_if_not(str.rbegin(), str.rend(), [](const char c)
                                                 {
                                                     return (isspace(c) != 0);
                                                 });
    if(strRItr != str.rend())
    {
        // Resize the string to remove blanks.
        const size_t sz = std::distance(str.rbegin(), strRItr);
        str.resize(str.size() - sz);
    }
}

// ------------------------------------------------------------------

void parseVertex(const ElemIDResult_t& elementIDRes)
{
    std::string args(&*elementIDRes.second);
    removeSurroundingBlanks(args);

    size_t processedCharsCount = 0;

    Vertex_t vtx;

    vtx.m_x = std::stof(args, &processedCharsCount);
    args.erase(args.cbegin(), args.cbegin() + processedCharsCount);

    vtx.m_y = std::stof(args, &processedCharsCount);
    args.erase(args.cbegin(), args.cbegin() + processedCharsCount);

    // Check if the 3rd parameter exists.
    if(args.size() > 0)
    {
        vtx.m_z = std::stof(args, &processedCharsCount);
    }

    switch(elementIDRes.first)
    {
    case ElementType::VERTEX : gVBuffer.push_back(vtx); gDocStats.m_VertexCount++; break;
    case ElementType::VERTEX_TEXTURE : gTexUVBuffer.push_back(vtx); break;
    case ElementType::VERTEX_NORMAL : gNormalBuffer.push_back(vtx); break;

    default: break;
    }
}

// ------------------------------------------------------------------

// Analyze the first vertices indices triplet.
VerticesIdxOrganization getIndicesOrganization(const std::string& args)
{
    // Initialize to no slashes found. So, no texture vertices, no normal vertices.
    VerticesIdxOrganization vtxIdxOrg = VerticesIdxOrganization::VGEO;

    const size_t doubleSlashesPos = args.find("//");
    if(doubleSlashesPos != std::string::npos)
    {
        // 2 contiguous slashes found, geometric vertex index and vertex normal index available.
        vtxIdxOrg = VerticesIdxOrganization::VGEO_VNORMAL;
    }
    else
    {
        CStringIterator_t firstBlankItr = std::find_if(args.cbegin(), args.cend(),
                                                 [](const char c)
                                                 {
                                                     return (isblank(c) != 0);
                                                 });
        const size_t countSlashes = std::count(args.cbegin(), firstBlankItr, '/');

        switch(countSlashes)
        {
        case 1 :
            // 1 slash found, geometric vertex index and vertex texture index available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO_VTEXTURE;
            break;

        case 2 :
            // 2 separate slashes found, all indices available.
            vtxIdxOrg = VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL;
            break;
        }
    }

    return (vtxIdxOrg);
}

void parseFace(const ElemIDResult_t& elementIDRes)
{
    std::string args(&*elementIDRes.second);
    removeSurroundingBlanks(args);

    // Get the organization of the vertices' indices triplets.
    const VerticesIdxOrganization vtxIdxOrg = getIndicesOrganization(args);

    // Remove all the slashes and keep only numbers.
    std::replace(args.begin(), args.end(), '/', ' ');

    size_t idxCount = 0;
    size_t pos = 0;

    while(pos < args.size())
    {
        args.erase(args.cbegin(), args.cbegin() + pos);
        const size_t vtxIdx = std::stol(args, &pos);
        gIdxBuffer.push_back(vtxIdx);
        ++idxCount;
    }

    gDocStats.m_FacesCount++;

    OBJASSERT(idxCount <= 12, "Face has too many vertices indices");

    ObjEntityFace objFace(gIdxBuffer.size() - idxCount, idxCount, vtxIdxOrg);
    gObjDB.insertEntity(objFace);
}

void parseGroup(const ElemIDResult_t& elementIDRes)
{
    std::string args(&*elementIDRes.second);
    removeSurroundingBlanks(args);

    if(elementIDRes.first == ElementType::GROUP || elementIDRes.first == ElementType::OBJECT_NAME)
    {
        // Only a group name points to an entity index.
        const size_t entityTableIdx = (elementIDRes.first == ElementType::GROUP) ?
                                      gObjDB.getEntitiesCount() : 0;
        ObjGroup grp(std::move(args), entityTableIdx, elementIDRes.first);
        gObjDB.insertGroup(std::move(grp));
    }
    else
        if(elementIDRes.first == ElementType::SMOOTHING_GROUP)
        {
            const size_t groupNum = std::stol(args);

            ObjGroup grp(groupNum, gObjDB.getEntitiesCount(), 0, elementIDRes.first);
            gObjDB.insertGroup(std::move(grp));
        }
}

// ------------------------------------------------------------------

ElemIDResult_t getElementType(const std::string& oneLine)
{
    // Skip white spaces at the beginning of the line (Only \t & ' ').
    CStringIterator_t lineStartItr = std::find_if(oneLine.cbegin(), oneLine.cend(),
                                               [](const char c)
                                               {
                                                   return (isblank(c) == 0);
                                               });

    // Skip this line if it is empty or is a comment line (\t, \f, \n, \r, \v,  ' ').
    if((isspace(*lineStartItr) != 0) || (*lineStartItr == '#'))
    {
        return (std::make_pair(ElementType::NONE, oneLine.cend()));
    }

    // Go to the next white space (Only \t & ' ').
    CStringIterator_t blankAfterTokenItr = std::find_if(lineStartItr, oneLine.cend(), &isblank);

    // Construct our element token.
    const std::string elementToken(lineStartItr, blankAfterTokenItr);

    // Find the element token in the keywords list.
    const auto keywordMatch = [&elementToken](const char* keyword)
                              {
                                  return (elementToken.compare(keyword) == 0);
                              };
    KeywordsArray_t::const_iterator keyItr = std::find_if(elementsKeywords.cbegin(),
                                                          elementsKeywords.cend(),
                                                          keywordMatch);
    if(keyItr == elementsKeywords.cend())
    {
        return (std::make_pair(ElementType::NONE, oneLine.cend()));
    }

    // Get the index of the identified element type and convert it to its enum correspondence.
    const KeywordsArray_t::difference_type elementTypeIdx = std::distance(elementsKeywords.cbegin(),
                                                                          keyItr);

    return (std::make_pair(static_cast<ElementType>(elementTypeIdx), blankAfterTokenItr));
}

void parseElementDataArgs(const ElemIDResult_t& elementIDRes)
{
    switch(elementIDRes.first)
    {
    case ElementType::VERTEX:
    case ElementType::VERTEX_TEXTURE:
    case ElementType::VERTEX_NORMAL:
    case ElementType::VERTEX_PARAM_SPACE:
        parseVertex(elementIDRes);
        break;

    case ElementType::FACE:
        parseFace(elementIDRes);
        break;

    case ElementType::GROUP:
    case ElementType::SMOOTHING_GROUP:
    case ElementType::MERGING_GROUP:
    // case ElementType::OBJECT_NAME:
        parseGroup(elementIDRes);
        break;

    default: break;
    }
}

/**
      *  @brief  Returns the size of a JSON object in the OCI world.
      *
      *  @param  rJsonVal JSON object which size will be calculated.
      *  @return the size of the JSON object in the OCI world.
      */
ElementType lastElement = ElementType::NONE;
void parseLine(const std::string& oneLine)
{
    ElemIDResult_t elemTypeRes = getElementType(oneLine);

    if(elemTypeRes.first != ElementType::NONE)
    {
        // Check if last element was a vertex (or its variants) and current element is not.
        const std::array<ElementType, 4> arr = {ElementType::VERTEX, ElementType::VERTEX_TEXTURE,
                                                ElementType::VERTEX_NORMAL,
                                                ElementType::VERTEX_PARAM_SPACE};
        if((lastElement != elemTypeRes.first) &&
           (std::find(arr.cbegin(), arr.cend(), lastElement) != arr.cend()) &&
           (std::find(arr.cbegin(), arr.cend(), elemTypeRes.first) == arr.cend()))
        {
            // Pre-allocate memory for next wave of vertices indices.
            gIdxBuffer.reserve(gVBuffer.size() + gTexUVBuffer.size() + gNormalBuffer.size());
        }

        lastElement = elemTypeRes.first;

        // Parse the arguments of each element.
        parseElementDataArgs(elemTypeRes);
    }
}

int main(void)
{
    const char* objFilePath = "/home/dartzon/Downloads/StorecastTest/cube.obj";
    const std::unique_ptr<std::FILE, decltype(&fclose)> smtObjFile(fopen(objFilePath, "r"),
                                                                   &fclose);
    if(smtObjFile == nullptr)
    {
        printf("Obj file not found !!!\n");
        return (1);
    }

    const uint16_t lineBufferSize = 256;
    char lineBuffer[lineBufferSize];

    std::string oneLine;
    oneLine.reserve(lineBufferSize);

    while(fgets(lineBuffer, lineBufferSize, smtObjFile.get()) != nullptr)
    {
        oneLine = lineBuffer;
        parseLine(oneLine);
    }

    gObjDB.sync(); //< IMPORTANT!!!!

    // printf("Information\n");
    // printf("\tVertex count\t%lu\n", gDocStats.m_VertexCount);
    // printf("\tFace count\t%lu\n", gDocStats.m_FacesCount);

    // ObjEntity* pObj = nullptr;

    // while((pObj = gObjDB.getNextEntity()) != nullptr)
    // {
    //     ObjEntityFace& face = *static_cast<ObjEntityFace*>(pObj);

    //     IndexBufferRange_t ibr = face.getVerticesIndices();

    //     printf("f");

    //     std::for_each(ibr.first, ibr.second,
    //                   [&face](const auto& val)
    //                   {
    //                       switch(face.getVerticesIndicesOrganization())
    //                       {
    //                       case VerticesIdxOrganization::VGEO: printf(" %lu", val); break;

    //                       case VerticesIdxOrganization::VGEO_VNORMAL: printf(" %lu//", val); break;

    //                       case VerticesIdxOrganization::VGEO_VTEXTURE: printf(" %lu/", val); break;

    //                       case VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL: printf(" %lu/", val);
    //                           break;

    //                       default: break;
    //                       }
    //                   });

    //     printf("\n");
    // }


    while(gObjDB.getGroupsCount() > 0)
    {
        ObjGroup grp = gObjDB.getNextGroup();

        // if(grp.getType() == ElementType::GROUP)
        // {
        //     OBJLOG("g ");
        // }
        // else
        // {
        //     OBJLOG("s ");
        // }

        OBJLOG(": ", grp.getFirstIncludedEntityIndex(), " -> ", grp.getIncludedEntitiesCount(), "\n");
    }

    return (0);
}
