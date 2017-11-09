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

void ObjDatabase::insertVertex(const Vertex_t& vtx, const ElementType eVtxType) noexcept
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

const VerticesRefList_t& ObjDatabase::getVerticesList(const WithVerticesIndices& elemWithVertices)
{
    const VerticesIdxOrganization vtxIdxOrg = elemWithVertices.getVerticesIndicesOrganization();
    const IndexBufferRange_t vtxIdxRange = elemWithVertices.getVerticesIndicesRange();

    const std::array<VertexBuffer_t*, 3> buffersPtrs = {&m_VBuffer, &m_TexUVBuffer,
                                                        &m_NormalBuffer};
    uint8_t bufferPtrIdx = 0;

    m_lastReturnedVertices.clear();

    std::for_each(m_IdxBuffer.cbegin() + vtxIdxRange.first,
                  m_IdxBuffer.cbegin() + vtxIdxRange.first + vtxIdxRange.second,
                  [this, vtxIdxOrg, &buffersPtrs, &bufferPtrIdx](const size_t& idxVtx)
                  {
                      VertexBuffer_t* currentBuff = buffersPtrs[bufferPtrIdx];;

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

                      m_lastReturnedVertices.push_back(&(*currentBuff)[idxVtx - 1]);
                  });

    return (m_lastReturnedVertices);
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

EntitiesRefRange_t ObjDatabase::getEntitiesInGroup(const ObjGroup& group) const
{
    const EntitiesIndexRange_t entIdxRange = group.getEntitiesIndicesRange();

    return(std::make_pair(m_entityTable.cbegin() + entIdxRange.first,
                          m_entityTable.cbegin() + entIdxRange.first + entIdxRange.second));
}

// =================================================================================================

void ObjDatabase::insertEntity(ObjEntity&& obj) noexcept
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
    std::for_each(m_currentGroupsIdx.cbegin(), m_currentGroupsIdx.cend(),
                  [this](const int64_t idx)
                  {
                      if(idx >= 0)
                      {
                          ++m_groupTable[idx];
                      }
                  });
}

// =================================================================================================

ObjEntity* ObjDatabase::getNextEntity(void) const noexcept
{
    ObjEntity* pObjEnt = nullptr;

    if(m_lastFetchedEntity != m_entityTable.cend())
    {
        pObjEnt = *m_lastFetchedEntity++;
    }

    return (pObjEnt);
}

// =================================================================================================

void ObjDatabase::insertGroup(ObjGroup&& grp) noexcept
{
    // Store this Obj group's index in the current Groups buffer.
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

    // Add the Obj group to the group table.
    m_groupTable.push_back(std::move(grp));
}

// =================================================================================================

ObjGroup* ObjDatabase::getNextGroup(void) const
{
    ObjGroup* pObjGrp = nullptr;

    if(m_lastFetchedGroup != m_groupTable.cend())
    {
        pObjGrp = &*m_lastFetchedGroup++;
    }

    return (pObjGrp);
}

// =================================================================================================

void ObjDatabase::sync(void)
{
    if(m_DBSynced == false)
    {
        // Copy Obj entities' addresses to the entity table.
        for(ObjEntityFace& face : m_facesTable)
        {
            m_entityTable.push_back(&face);
        }

        m_lastFetchedEntity = m_entityTable.begin();
        m_lastFetchedGroup = m_groupTable.begin();

        m_DBSynced = true;
    }
}

