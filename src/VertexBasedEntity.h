/// MIT License
///
/// Copyright (c) 2017 Othmane AIT EL CADI
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

/// \file      VertexBasedEntity.h
///
/// \brief     Obj entity with vertices indices. Any Obj entity with vertices indices such as
///            (face), should inherit from this class.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      12-04-2019

#ifndef VERTEXBASEDENTITY_H_
#define VERTEXBASEDENTITY_H_

#include "Types.h"

/// \brief Obj entity with vertices indices.
class VertexBasedEntity
{
public:
    /// \brief  Constructor.
    ///
    /// \param  firstIdx First vertex index.
    /// \param  lastIdx Last vertex index.
    /// \param  eVtxIdxOrganization Vertices indices layout.
    VertexBasedEntity(const size_t firstIdx,
                      const size_t lastIdx,
                      const VerticesIdxOrganization eVtxIdxOrganization) :
        m_firstIdx(firstIdx),
        m_lastIdx(lastIdx), m_eVtxIdxOrganization(eVtxIdxOrganization)
    {
    }

    /// \brief  Default copy constructor.
    VertexBasedEntity(const VertexBasedEntity&) = default;

    /// \brief  Default move constructor.
    VertexBasedEntity(VertexBasedEntity&&) noexcept = default;

    /// \brief  Returns a pair representing a range of vertices indices.
    ///
    /// \return Pair of vertices indices [start, end].
    IndexBufferRange_t getVerticesIndicesRange() const
    {
        return std::make_pair(m_firstIdx, m_lastIdx);
    }

    // Accessors ===================================================================================

    size_t getFirstVertexIndex() const { return m_firstIdx; }
    size_t getLastVertexIndex() const { return m_lastIdx; }
    VerticesIdxOrganization getVerticesIndicesOrganization() const { return m_eVtxIdxOrganization; }

private:
    // Members =====================================================================================

    size_t m_firstIdx;                              ///< First index in the index buffer.
    size_t m_lastIdx;                               ///< Last index in the index buffer.
    VerticesIdxOrganization m_eVtxIdxOrganization;  ///< Describes how indices are organized.
};

#endif /* VERTEXBASEDENTITY_H_ */
