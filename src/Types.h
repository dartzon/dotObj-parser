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
 * \file      TypeDefs.h
 *
 * \brief     Type definitions.
 * \details   Containers, and iterators type definitions for flexibility and ease of use.
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

// Represents a 2D/3D coordinates.
struct Coordinates3D;
using Vertex_t = Coordinates3D;

// Vertices buffer type.
using VertexBuffer_t = std::vector<Vertex_t>;
// Vertices textures U/V buffer type.
using TextureUVBuffer_t = std::vector<Vertex_t>;
// Vertices normals buffer type.
using NormalBuffer_t = std::vector<Vertex_t>;

// Vertices indices buffer type.
using IndexBuffer_t = std::vector<size_t>;
// Range of vertices indices type.
using IndexBufferRange_t = std::pair<IndexBuffer_t::const_iterator,
                                     IndexBuffer_t::const_iterator>;

// Wavefront Obj keyword dictionary type.
template<size_t sz> using ObjKeywords_t = std::array<const char*, sz>;
using KeywordsArray_t = ObjKeywords_t<35>;

// std::string iterators types.
using CStringIterator_t = std::string::const_iterator;
using RStringIterator_t = std::string::reverse_iterator;

// Element type + std::string iterator to its 1st parameter.
// Since C++11 std::string is contiguous in memory, it's safe to create one from &*iterator.
enum class ElementType : int8_t;
using ElemIDResult_t = std::pair<ElementType, CStringIterator_t>;

/* ============================================================================================== */

/**
  *  @brief Standard 3-dimensional coordinates.
  */
struct Coordinates3D
{
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;
};

/* ============================================================================================== */

/**
  *  @brief .obj file's elements' types.
  */
enum class ElementType : int8_t
{
    NONE = -1,            //< ??
    VERTEX = 0,           //< v
    VERTEX_TEXTURE,       //< vt
    VERTEX_NORMAL,        //< vn
    VERTEX_PARAM_SPACE,   //< vp
    POINT = 8,            //< p
    LINE,                 //< l
    FACE,                 //< f
    GROUP = 21,           //< g
    SMOOTHING_GROUP,      //< s
    MERGING_GROUP,        //< mg
    OBJECT_NAME           //< o
};

/* ============================================================================================== */

/**
  *  @brief Some elements, such as faces and surfaces, may have a triplet of
  *         numbers that reference vertex data.These numbers are the reference
  *         numbers for a geometric vertex, a texture vertex, and a vertex normal.
  */
enum class VerticesIdxOrganization : int8_t
{
    NONE = -1,             //< No indices.
    VGEO = 0,              //< Geometric vertex index only.
    VGEO_VTEXTURE,         //< Geometric vertex + Vertex texture.
    VGEO_VNORMAL,          //< Geometric vertex + Vertex normal.
    VGEO_VTEXTURE_VNORMAL  //< Geometric vertex + Vertex texture + Vertex normal.
};

/* ============================================================================================== */

/**
  *  @brief .obj file's stats.
  */
struct DocumentStats
{
    size_t m_VertexCount = 0;
    size_t m_FacesCount = 0;
    size_t m_TriangleCount = 0;
    size_t m_QuadCount = 0;
};

#endif /* __TYPEDEFS_H__ */
