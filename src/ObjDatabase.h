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

/// \file      ObjDatabase.h
///
/// \brief     Obj entities database. Contains all of the components of and Obj file.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#ifndef OBJDATABASE_H_
#define OBJDATABASE_H_

#include "Types.h"

#include "ObjEntityFace.h"
#include "ObjGroup.h"

#include <queue>

/// \brief Obj entities database.
class ObjDatabase
{
public:
    /// \brief  Default ctor.
    ObjDatabase() = default;

    /// \brief  Deleted copy ctor, we only need one Obj Database instance.
    ObjDatabase(const ObjDatabase&) = delete;

    /// \brief  Default move ctor.
    ObjDatabase(ObjDatabase&&) = default;

    /// \brief  Deleted assignment operator, we only need one Obj Database instance.
    ObjDatabase& operator=(const ObjDatabase&) = delete;

    /// \brief  Default move assignment operator.
    ObjDatabase& operator=(ObjDatabase&&) = default;

    /// \brief  Insert a Vertex.
    ///
    /// \param  vtx Vertex to be inserted.
    /// \param  eVtxType Vertex type.
    void insertVertex(const Vertex_t& vtx, const ElementType eVtxType);

    /// \brief  Return a list of vertices corresponding to the entity passed as a parameter.
    ///
    /// \param  elemWithVertices Reference to Obj entity.
    /// \return  Const reference to a vector of pointers to vertices.
    VerticesRefList_t getVerticesList(const WithVerticesIndices& elemWithVertices) const;

    /// \brief  Return a pair of iterators to the first and the last vertices indices.
    ///
    /// \param  elemWithVertices Reference to Obj entity.
    /// \return  pair of iterators to the first and the last vertices indices.
    IndexBufferRefRange_t getVerticesIndicesList(const WithVerticesIndices& elemWithVertices) const;

    /// \brief  Return a list entities included in an Obj Group.
    ///
    /// \param  group Concerned group.
    /// \return range of entites.
    FacesRefRange_t getEntitiesInGroup(const ObjGroup& group) const;

    /// \brief  Insert a new Obj entity. The object will be moved.
    ///
    /// \param  obj Obj entity to be inserted.
    /// \param  currentGroupsIdx array of the currently active groups.
    void insertEntity(ObjEntity& obj, const std::array<int64_t, 3>& currentGroupsIdx);

    /// \brief  Insert a new Obj entity. The object will be moved.
    ///
    /// \param  obj Obj entity to be inserted.
    void insertGroup(ObjGroup& grp) { m_groupTable.push_back(std::move(grp)); }

    /// \brief  Insert a new index in the index buffer.
    ///
    /// \param  obj Obj entity to be inserted.
    void insertIndex(const size_t idx) { m_IdxBuffer.push_back(idx); }

    /// \brief  Pre-allocate memory for the next wave of vertices indices.
    void reserveIndexBufferMemory()
    {
        m_IdxBuffer.reserve(m_VBuffer.size() + m_TexUVBuffer.size() + m_NormalBuffer.size());
    }

    // Iterators functions =========================================================================

    FacesBuffer_t::iterator begin() noexcept { return m_facesTable.begin(); }
    FacesBuffer_t::iterator end() noexcept { return m_facesTable.end(); }

    FacesBuffer_t::const_iterator begin() const noexcept { return m_facesTable.begin(); }
    FacesBuffer_t::const_iterator end() const noexcept { return m_facesTable.end(); }

    FacesBuffer_t::const_iterator cbegin() const noexcept { return m_facesTable.cbegin(); }
    FacesBuffer_t::const_iterator cend() const noexcept { return m_facesTable.cend(); }

    using VertexIterator = VertexBuffer_t::iterator;
    using VertexConstIterator = VertexBuffer_t::const_iterator;
    friend VertexIterator vertexBegin(ObjDatabase& db) noexcept;
    friend VertexIterator vertexEnd(ObjDatabase& db) noexcept;
    friend VertexConstIterator vertexBegin(const ObjDatabase& db) noexcept;
    friend VertexConstIterator vertexEnd(const ObjDatabase& db) noexcept;
    friend VertexConstIterator vertexCBegin(const ObjDatabase& db) noexcept;
    friend VertexConstIterator vertexCEnd(const ObjDatabase& db) noexcept;

    using GroupsIterator = GroupsBuffer_t::iterator;
    using GroupsConstIterator = GroupsBuffer_t::const_iterator;
    friend GroupsIterator groupBegin(ObjDatabase& db) noexcept;
    friend GroupsIterator groupEnd(ObjDatabase& db) noexcept;
    friend GroupsConstIterator groupBegin(const ObjDatabase& db) noexcept;
    friend GroupsConstIterator groupEnd(const ObjDatabase& db) noexcept;
    friend GroupsConstIterator groupCBegin(const ObjDatabase& db) noexcept;
    friend GroupsConstIterator groupCEnd(const ObjDatabase& db) noexcept;

    // Accessors ===================================================================================

    inline size_t getGroupsCount() const { return m_groupTable.size(); }
    inline size_t getIndexBufferCount() const { return m_IdxBuffer.size(); }
    inline size_t getTextureUVIndexBufferCount() const { return m_TexUVBuffer.size(); }
    inline size_t getVertexNormalIndexBufferCount() const { return m_NormalBuffer.size(); }
    inline size_t getEntitiesCount() const { return m_totalEntitiesCount; }

    inline bool isEmpty() const { return m_facesTable.size() == 0; }

private:
    // Members =====================================================================================

    VertexBuffer_t m_VBuffer;         ///< Vertex buffer.
    TextureUVBuffer_t m_TexUVBuffer;  ///< Vertex texture buffer.
    NormalBuffer_t m_NormalBuffer;    ///< Vertex normal buffer.

    IndexBuffer_t m_IdxBuffer;  ///< Index buffer.

    FacesBuffer_t m_facesTable;  ///< Vector of Face entities.

    GroupsBuffer_t m_groupTable;  ///< Vector of Groups.

    size_t m_totalEntitiesCount = 0;  ///< Total number of entities.
};

// Iterators free functions ========================================================================

ObjDatabase::VertexIterator vertexBegin(ObjDatabase& db) noexcept;
ObjDatabase::VertexIterator vertexEnd(ObjDatabase& db) noexcept;
ObjDatabase::VertexConstIterator vertexBegin(const ObjDatabase& db) noexcept;
ObjDatabase::VertexConstIterator vertexEnd(const ObjDatabase& db) noexcept;
ObjDatabase::VertexConstIterator vertexCBegin(const ObjDatabase& db) noexcept;
ObjDatabase::VertexConstIterator vertexCEnd(const ObjDatabase& db) noexcept;

ObjDatabase::GroupsIterator groupBegin(ObjDatabase& db) noexcept;
ObjDatabase::GroupsIterator groupEnd(ObjDatabase& db) noexcept;
ObjDatabase::GroupsConstIterator groupBegin(const ObjDatabase& db) noexcept;
ObjDatabase::GroupsConstIterator groupEnd(const ObjDatabase& db) noexcept;
ObjDatabase::GroupsConstIterator groupCBegin(const ObjDatabase& db) noexcept;
ObjDatabase::GroupsConstIterator groupCEnd(const ObjDatabase& db) noexcept;

#endif /* OBJDATABASE_H_ */
