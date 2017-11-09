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
 * \file      ObjEntityFace.h
 *
 * \brief     Face entity.
 * \details   Represents an Obj Face with its vertices indices list.
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __OBJENTITYFACE_H__
#define __OBJENTITYFACE_H__

#include "ObjEntity.h"
#include "WithVerticesIndices.h"

class ObjEntityFace : public ObjEntity, public WithVerticesIndices
{
public:

    /**
      *  @brief  Constructor for (g/o).
      *
      *  @param  indexBufferIdx Index of the first vertex index.
      *  @param  indicesOffset Count of vertices indices.
      *  @param  eParamsOrganization Vertices indices layout.
      */
    ObjEntityFace(const size_t indexBufferIdx, const size_t indicesOffset,
                  const VerticesIdxOrganization eParamsOrganization);

    ObjEntityFace(const ObjEntityFace&) = default;
    ObjEntityFace(ObjEntityFace&&) = default;


    // Accessors ===================================================================================

    inline bool isTriangle(void) const
    {
        return (m_isTriangle);
    }

private:

    // Members =====================================================================================

    bool m_isTriangle;    //< Is this face a triangle?
};

#endif /* __OBJENTITYFACE_H__ */
