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
 * \file      WithVerticesIndices.h
 *
 * \brief     <brief description>
 * \details   <detailed description>
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __WITHVERTICESINDICES_H__
#define __WITHVERTICESINDICES_H__

#include <utility>
#include "Types.h"

class WithVerticesIndices
{
public:
    WithVerticesIndices(const size_t indexBufferIdx, const size_t indicesOffset,
                        const VerticesIdxOrganization eParamsOrganization) :
        m_indexBufferIdx(indexBufferIdx), m_indicesOffset(indicesOffset),
        m_eParamsOrganization(eParamsOrganization)
    {
    }

    /**
      *  @brief  Returns a pair representing a range of vertices indices.

      *  @return Pair of vertices indices [start, end].
      */
    IndexBufferRange_t getVerticesIndices(void) const
    {
        // TODO: make gIdxBuffer non-global.

        // IndexBufferRange_t res; = std::make_pair(gIdxBuffer.cbegin() + m_indexBufferIdx,
        //                                         gIdxBuffer.cbegin() + m_indexBufferIdx +
        //                                         m_indicesOffset);

        IndexBufferRange_t res;
        return (res);
    }

    // Accessors ===================================================================================

    size_t getIndexBufferStart(void) const
    {
        return (m_indexBufferIdx);
    }
    size_t getIndicesCount(void) const
    {
        return (m_indicesOffset);
    }
    VerticesIdxOrganization getVerticesIndicesOrganization(void) const
    {
        return (m_eParamsOrganization);
    }

private:

    // Members =====================================================================================

    // IndexBuffer_t& m_IdxBuffer;                     //< Reference to the index buffer.
    size_t m_indexBufferIdx;                        //< Start index in the index buffer.
    size_t m_indicesOffset;                         //< Count of indices for this face.
    VerticesIdxOrganization m_eParamsOrganization;  //< Describes how indices are organized.
};

#endif /* __WITHVERTICESINDICES_H__ */
