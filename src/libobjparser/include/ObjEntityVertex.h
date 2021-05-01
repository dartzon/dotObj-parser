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

/// \file      ObjEntityVertex.h
///
/// \brief     Vertex entity. Represents an Obj Vertex.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      15-04-2019

#ifndef OBJVERTEX_H_
#define OBJVERTEX_H_

#include "ObjEntity.h"

#include <utility>

/// \brief Represents an Obj Vertex.
struct ObjEntityVertex : public ObjEntity
{
    explicit ObjEntityVertex(const ElementType entityType) : ObjEntity(entityType) {}

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;
    float m_w = 1.0f;  ///< Weight for rational curves and surfaces.

    friend ObjDatabase;
};

template<>
struct std::tuple_size<ObjEntityVertex>
{
    static constexpr int value = 4;  ///< ObjEntityVertex has 4 attributes.
};

template<std::size_t idx>
struct std::tuple_element<idx, ObjEntityVertex>
{
    using type = float;  ///< ObjEntityVertex's attributes are float.
};

// ObjEntityVertex's attributes getters.
template<std::size_t Idx>
auto get(const ObjEntityVertex& vtx)
{
    static_assert(Idx <= 3);

    if constexpr (Idx == 0)
    {
        return vtx.m_x;
    }
    else if constexpr (Idx == 1)
    {
        return vtx.m_y;
    }
    else if constexpr (Idx == 2)
    {
        return vtx.m_z;
    }
    else
    {
        return vtx.m_w;
    }
}

template<std::size_t Idx>
auto& get(ObjEntityVertex& vtx)
{
    static_assert(Idx <= 3);

    if constexpr (Idx == 0)
    {
        return vtx.m_x;
    }
    else if constexpr (Idx == 1)
    {
        return vtx.m_y;
    }
    else if constexpr (Idx == 2)
    {
        return vtx.m_z;
    }
    else
    {
        return vtx.m_w;
    }
}

template<std::size_t Idx>
auto& get(ObjEntityVertex&& vtx)
{
    static_assert(Idx <= 3);

    if constexpr (Idx == 0)
    {
        return vtx.m_x;
    }
    else if constexpr (Idx == 1)
    {
        return vtx.m_y;
    }
    else if constexpr (Idx == 2)
    {
        return vtx.m_z;
    }
    else
    {
        return vtx.m_w;
    }
}

#endif /* OBJVERTEX_H_ */
