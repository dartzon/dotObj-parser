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

/// \file      ObjEntityFace.cpp
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#include "ObjEntityFace.h"

ObjEntityFace::ObjEntityFace(const size_t firstIdx,
                             const size_t lastIdx,
                             const VerticesIdxOrganization eVtxIdxOrganization) :
    ObjEntity(ElementType::FACE),
    VertexBasedEntity(firstIdx, lastIdx, eVtxIdxOrganization)
{
    m_isTriangle = (((lastIdx - firstIdx + 1) % 3) == 0);
    m_hasTextureVertex = (eVtxIdxOrganization == VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL);
    m_hasVertexNormal = (eVtxIdxOrganization == VerticesIdxOrganization::VGEO_VNORMAL) ||
                        (eVtxIdxOrganization == VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL);
}
