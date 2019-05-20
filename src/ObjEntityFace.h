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
#include "VertexBasedEntity.h"

/// \brief Represents an Obj Face with its vertices indices list.
class ObjEntityFace
    : public ObjEntity
    , public VertexBasedEntity
{
public:
    /// \brief  Constructor.
    ///
    /// \param  firstIdx First vertex index.
    /// \param  lastIdx Last vertex index.
    /// \param  eParamsOrganization Vertices indices layout.
    ObjEntityFace(const size_t firstIdx,
                  const size_t lastIdx,
                  const VerticesIdxOrganization eVtxIdxOrganization);

    /// \brief Default copy constructor.
    ObjEntityFace(const ObjEntityFace&) = default;

    /// \brief  Default move constructor.
    ObjEntityFace(ObjEntityFace&& face) noexcept = default;

    // Accessors ===================================================================================

    bool isTriangle() const { return m_isTriangle; }
    bool hasTextureVertex() const { return m_hasTextureVertex; }
    bool hasNormal() const { return m_hasVertexNormal; }

private:
    // Members =====================================================================================

    bool m_isTriangle;        ///< Is this face a triangle?
    bool m_hasTextureVertex;  ///< Does the face has a texture vertex?
    bool m_hasVertexNormal;   ///< Does the face has a vertex normal?
};

// Typedefs ========================================================================================
using FaceBuffer_t = std::vector<ObjEntityFace>;
using FacesRefRange_t = std::pair<FaceBuffer_t::const_iterator, FaceBuffer_t::const_iterator>;

#endif /* OBJENTITYFACE_H_ */
