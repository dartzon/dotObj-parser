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

/// \file      WithVerticesIndices.h
///
/// \brief     Obj entity with vertices indices. Any Obj entity with vertices indices such as
///            (face), should inherit from this class.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#ifndef WITHVERTICESINDICES_H_
#define WITHVERTICESINDICES_H_

#include "Types.h"

/// \brief Obj entity with vertices indices.
class WithVerticesIndices
{
public:
    /// \brief  Constructor.
    ///
    /// \param  indexBufferIdx Index of the first vertex index.
    /// \param  indicesOffset Count of vertices indices.
    /// \param  eParamsOrganization Vertices indices layout.
    WithVerticesIndices(const size_t indexBufferIdx,
                        const size_t indicesOffset,
                        const VerticesIdxOrganization eParamsOrganization) :
        m_indexBufferIdx(indexBufferIdx),
        m_indicesOffset(indicesOffset), m_eParamsOrganization(eParamsOrganization)
    {
    }

    /// \brief  Default move constructor.
    WithVerticesIndices(WithVerticesIndices&&) noexcept = default;

    /// \brief  Returns a pair representing a range of vertices indices.
    ///
    /// \return Pair of vertices indices [start, end].
    IndexBufferRange_t getVerticesIndicesRange() const
    {
        return std::make_pair(m_indexBufferIdx, m_indicesOffset);
    }

    // Accessors ===================================================================================

    inline size_t getIndexBufferStart() const { return m_indexBufferIdx; }
    inline size_t getIndicesCount() const { return m_indicesOffset; }
    inline VerticesIdxOrganization getVerticesIndicesOrganization() const
    {
        return m_eParamsOrganization;
    }

private:
    // Members =====================================================================================

    size_t m_indexBufferIdx;                        ///< Start index in the index buffer.
    size_t m_indicesOffset;                         ///< Count of indices for this face.
    VerticesIdxOrganization m_eParamsOrganization;  ///< Describes how indices are organized.
};

#endif /* WITHVERTICESINDICES_H_ */
