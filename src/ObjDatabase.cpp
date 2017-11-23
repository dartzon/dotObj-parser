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
 * \file      ObjDatabase.cpp
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#include "ObjDatabase.h"

#include "Utils.h"
#include <algorithm>

void ObjDatabase::insertVertex(const Vertex_t& vtx, const ElementType eVtxType)
{
    switch(eVtxType)
    {
    case ElementType::VERTEX : m_VBuffer.push_back(vtx); break;
    case ElementType::VERTEX_TEXTURE : m_TexUVBuffer.push_back(vtx); break;
    case ElementType::VERTEX_NORMAL : m_NormalBuffer.push_back(vtx); break;

    default: break;
    }
}

// =================================================================================================

VerticesRefList_t ObjDatabase::getVerticesList(const WithVerticesIndices& elemWithVertices) const
{
    const VerticesIdxOrganization vtxIdxOrg = elemWithVertices.getVerticesIndicesOrganization();
    const IndexBufferRange_t vtxIdxRange = elemWithVertices.getVerticesIndicesRange();

    const std::array<const VertexBuffer_t*, 3> buffersPtrs = {&m_VBuffer, &m_TexUVBuffer,
                                                              &m_NormalBuffer};
    uint8_t bufferPtrIdx = 0;
    VerticesRefList_t verticesRefList;

    std::for_each(m_IdxBuffer.cbegin() + vtxIdxRange.first,
                  m_IdxBuffer.cbegin() + vtxIdxRange.first + vtxIdxRange.second,
                  [vtxIdxOrg, &buffersPtrs, &bufferPtrIdx, &verticesRefList](const size_t& idxVtx)
                  {
                      const VertexBuffer_t* pCurrentBuff = buffersPtrs[bufferPtrIdx];;

                      switch(vtxIdxOrg)
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

                      verticesRefList.push_back(&(*pCurrentBuff)[idxVtx - 1]);
                  });

    return (verticesRefList);
}

// =================================================================================================

IndexBufferRefRange_t ObjDatabase::getVerticesIndicesList(const WithVerticesIndices&
                                                          elemWithVertices) const
{
    const IndexBufferRange_t vtxIdxRange = elemWithVertices.getVerticesIndicesRange();

    return(std::make_pair(m_IdxBuffer.cbegin() + vtxIdxRange.first,
                          m_IdxBuffer.cbegin() + vtxIdxRange.first + vtxIdxRange.second));
}

// =================================================================================================

FacesRefRange_t ObjDatabase::getEntitiesInGroup(const ObjGroup& group) const
{
    const EntitiesIndexRange_t entIdxRange = group.getEntitiesIndicesRange();

    return(std::make_pair(m_facesTable.cbegin() + entIdxRange.first,
                          m_facesTable.cbegin() + entIdxRange.first + entIdxRange.second));
}

// =================================================================================================

void ObjDatabase::insertEntity(ObjEntity& obj, const std::array<int64_t, 3>& currentGroupsIdx)
{
    // Insert the Obj entity in its proper container.
    switch(obj.getType())
    {
    case ElementType::FACE:
        obj.setID(m_facesTable.size() + 1);
        m_facesTable.push_back(static_cast<ObjEntityFace&&>(obj));
        // Increment entities count.
        ++m_totalEntitiesCount;
        break;

    default: break;
    };

    // Update the count of included Obj entities for each current Obj group.
    std::for_each(currentGroupsIdx.cbegin(), currentGroupsIdx.cend(),
                  [this](const int64_t idx)
                  {
                      if(idx >= 0)
                      {
                          ++m_groupTable[idx];
                      }
                  });
}

// Iterators free functions ========================================================================
ObjDatabase::VertexIterator vertexBegin(ObjDatabase& db) noexcept
{
    return (db.m_VBuffer.begin());
}
ObjDatabase::VertexIterator vertexEnd(ObjDatabase& db) noexcept
{
    return (db.m_VBuffer.end());
}
ObjDatabase::VertexConstIterator vertexBegin(const ObjDatabase& db) noexcept
{
    return (db.m_VBuffer.cbegin());
}
ObjDatabase::VertexConstIterator vertexEnd(const ObjDatabase& db) noexcept
{
    return (db.m_VBuffer.cend());
}
ObjDatabase::VertexConstIterator vertexCBegin(const ObjDatabase& db) noexcept
{
    return (db.m_VBuffer.cbegin());
}
ObjDatabase::VertexConstIterator vertexCEnd(const ObjDatabase& db) noexcept
{
    return (db.m_VBuffer.cend());
}


ObjDatabase::GroupsIterator groupBegin(ObjDatabase& db) noexcept
{
    return (db.m_groupTable.begin());
}
ObjDatabase::GroupsIterator groupEnd(ObjDatabase& db) noexcept
{
    return (db.m_groupTable.end());
}
ObjDatabase::GroupsConstIterator groupBegin(const ObjDatabase& db) noexcept
{
    return (db.m_groupTable.cbegin());
}
ObjDatabase::GroupsConstIterator groupEnd(const ObjDatabase& db) noexcept
{
    return (db.m_groupTable.cend());
}
ObjDatabase::GroupsConstIterator groupCBegin(const ObjDatabase& db) noexcept
{
    return (db.m_groupTable.cbegin());
}
ObjDatabase::GroupsConstIterator groupCEnd(const ObjDatabase& db) noexcept
{
    return (db.m_groupTable.cend());
}
