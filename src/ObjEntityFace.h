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

/// \file      ObjEntityFace.h
///
/// \brief     Face entity. Represents an Obj Face with its vertices indices list.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#ifndef OBJENTITYFACE_H_
#define OBJENTITYFACE_H_

#include "ObjEntity.h"
#include "WithVerticesIndices.h"

/// \brief Represents an Obj Face with its vertices indices list.
class ObjEntityFace
    : public ObjEntity
    , public WithVerticesIndices
{
public:
    /// \brief  Constructor for (g/o).
    ///
    /// \param  indexBufferIdx Index of the first vertex index.
    /// \param  indicesOffset Count of vertices indices.
    /// \param  eParamsOrganization Vertices indices layout.
    ObjEntityFace(const size_t indexBufferIdx,
                  const size_t indicesOffset,
                  const VerticesIdxOrganization eParamsOrganization);

    /// \brief  Move constructor.
    ObjEntityFace(ObjEntityFace&& face) noexcept :
        ObjEntity(std::move(face)), WithVerticesIndices(std::move(face))
    {
    }

    // Accessors ===================================================================================

    inline bool isTriangle() const { return m_isTriangle; }

private:
    // Members =====================================================================================

    bool m_isTriangle;  ///< Is this face a triangle?
};

// Typedefs ========================================================================================
using FacesBuffer_t = std::vector<ObjEntityFace>;
using FacesRefRange_t = std::pair<FacesBuffer_t::const_iterator, FacesBuffer_t::const_iterator>;

#endif /* OBJENTITYFACE_H_ */
