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

/// \file      ObjDatabase.cpp
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#include "ObjDatabase.h"

#include "Utils.h"
#include <algorithm>

VerticesRefList_t ObjDatabase::getVerticesList(const VertexBasedEntity& elemWithVertices) const
{
    const auto [rangeBegin, rangeEnd] = elemWithVertices.getVerticesIndicesRange();
    const VerticesIdxOrganization vtxIdxOrg = elemWithVertices.getVerticesIndicesOrganization();

    VerticesRefList_t vtxList;

    std::for_each(m_IdxBuffer.cbegin() + rangeBegin,
                  m_IdxBuffer.cbegin() + rangeBegin + rangeEnd,
                  [this, vtxIdxOrg, &vtxList](const size_t vtxIdx) {
                      constexpr uint8_t vtxBuffIdx = 0, vtxTextureIdx = 1, vtxNormalIdx = 2;

                      switch (vtxIdxOrg)
                      {
                      case VerticesIdxOrganization::VGEO:
                          vtxList.push_back(m_vertexBuffer[vtxBuffIdx][vtxIdx]);
                          break;
                      case VerticesIdxOrganization::VGEO_VTEXTURE:
                          vtxList.push_back(m_vertexBuffer[vtxBuffIdx][vtxIdx]);
                          vtxList.push_back(m_vertexBuffer[vtxTextureIdx][vtxIdx]);
                          break;
                      case VerticesIdxOrganization::VGEO_VNORMAL:
                          vtxList.push_back(m_vertexBuffer[vtxBuffIdx][vtxIdx]);
                          vtxList.push_back(m_vertexBuffer[vtxNormalIdx][vtxIdx]);
                          break;
                      case VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL:
                          vtxList.push_back(m_vertexBuffer[vtxBuffIdx][vtxIdx]);
                          vtxList.push_back(m_vertexBuffer[vtxTextureIdx][vtxIdx]);
                          vtxList.push_back(m_vertexBuffer[vtxNormalIdx][vtxIdx]);
                          break;
                      }
                  });

    return vtxList;
}

// =================================================================================================

IndexBufferRangeIterators_t
ObjDatabase::getVerticesIterators(const VertexBasedEntity& elemWithVertices) const
{
    const IndexBufferRange_t vtxIdxRange = elemWithVertices.getVerticesIndicesRange();

    return std::make_pair(m_IdxBuffer.cbegin() + vtxIdxRange.first,
                          m_IdxBuffer.cbegin() + vtxIdxRange.second + 1);
}

// =================================================================================================

EntitiesRefList_t ObjDatabase::getEntitiesInGroup(const ObjEntityGroup& group) const
{
    const std::vector<EntitiesIndexRange_t>& entIdxRange = group.getEntitiesIndicesRange();

    EntitiesRefList_t includedEntities;

    std::for_each(entIdxRange.cbegin(),
                  entIdxRange.cend(),
                  [this, &includedEntities](const EntitiesIndexRange_t& range) {
                      const auto [begin, end] = range;

                      includedEntities.reserve(end - begin + 1);

                      std::copy(m_allEntitiesTable.cbegin() + begin,
                                m_allEntitiesTable.cend() + begin + end,
                                std::back_inserter(includedEntities));
                  });

    return includedEntities;
}
