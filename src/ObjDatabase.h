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
 * \file      ObjDatabase.h
 *
 * \brief     Obj entities database.
 * \details   Contains all of the components of and Obj file.
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __OBJDATABASE_H__
#define __OBJDATABASE_H__

#include "Types.h"

#include "ObjEntityFace.h"
#include "ObjGroup.h"

#include <queue>

class ObjDatabase
{
public:

    /**
      *  @brief  Default ctor.
      */
    ObjDatabase(void) = default;
    /**
      *  @brief  Deleted copy ctor, we only need one Obj Database instance.
      */
    ObjDatabase(const ObjDatabase&) = delete;
    /**
      *  @brief  Default move ctor.
      */
    ObjDatabase(ObjDatabase&&) = default;
    /**
      *  @brief  Deleted assignment operator, we only need one Obj Database instance.
      */
    ObjDatabase& operator=(const ObjDatabase&) = delete;
    /**
      *  @brief  Default move assignment operator.
      */
    ObjDatabase& operator=(ObjDatabase&&) = default;

    /**
      *  @brief  Insert a Vertex.
      *
      *  @param  vtx Vertex to be inserted.
      *  @param  eVtxType Vertex type.
      */
    void insertVertex(const Vertex_t& vtx, const ElementType eVtxType);

    /**
      *  @brief  Return a list of vertices corresponding to the entity passed as a parameter.
      *
      *  @param  elemWithVertices Reference to Obj entity.
      *  @return  Const reference to a vector of pointers to vertices.
      */
    const VerticesRefList_t& getVerticesList(const WithVerticesIndices& elemWithVertices);

    /**
      *  @brief  Return a pair of iterators to the first and the last vertices indices.
      *
      *  @param  elemWithVertices Reference to Obj entity.
      *  @return  pair of iterators to the first and the last vertices indices.
      */
    IndexBufferRefRange_t getVerticesIndicesList(const WithVerticesIndices& elemWithVertices) const;

    /**
      *  @brief  Return a list of pointers to entities included in an Obj Group.
      *
      *  @param  group Concerned group.
      *  @return  Const reference to a vector of pointers to vertices.
      */
    EntitiesRefRange_t getEntitiesInGroup(const ObjGroup& group) const;

    /**
      *  @brief  Insert a new Obj entity. The object will be moved.
      *
      *  @param  obj Obj entity to be inserted.
      */
    void insertEntity(ObjEntity&& obj);

    /**
      *  @brief  Insert a new Obj entity. The object will be moved.
      *
      *  @param  obj Obj entity to be inserted.
      */
    void insertGroup(ObjGroup&& grp);

    /**
      *  @brief  Get the next group in the database.
      *  @return Pointer to next Group.
      */
    ObjGroup* getNextGroup(void) const;

    /**
      *  @brief  Insert a new index in the index buffer.
      *
      *  @param  obj Obj entity to be inserted.
      */
    inline void insertIndex(const size_t idx)
    {
        m_IdxBuffer.push_back(idx);
    }

    /**
      *  @brief  Notify that the Obj file parsing is over.
      */
    inline void parsingFinished(void)
    {
        sync();
    }

    /**
      *  @brief  Pre-allocate memory for the next wave of vertices indices.
      */
    inline void reserveIndexBufferMemory(void)
    {
        m_IdxBuffer.reserve(m_VBuffer.size() + m_TexUVBuffer.size() + m_NormalBuffer.size());
    }

    // Iterators functions =========================================================================

    using FacesBuffer_t = std::vector<ObjEntityFace>;
    FacesBuffer_t::iterator begin(void) noexcept
    {
        return (m_facesTable.begin());
    }
    FacesBuffer_t::iterator end(void) noexcept
    {
        return (m_facesTable.end());
    }

    FacesBuffer_t::const_iterator begin(void) const noexcept
    {
        return (m_facesTable.begin());
    }
    FacesBuffer_t::const_iterator end(void) const noexcept
    {
        return (m_facesTable.end());
    }

    FacesBuffer_t::const_iterator cbegin(void) const noexcept
    {
        return (m_facesTable.cbegin());
    }
    FacesBuffer_t::const_iterator cend(void) const noexcept
    {
        return (m_facesTable.cend());
    }

    // Accessors ===================================================================================

    inline size_t getGroupsCount(void) const
    {
        return (m_groupTable.size());
    }
    inline size_t getIndexBufferCount(void) const
    {
        return (m_IdxBuffer.size());
    }
    inline size_t getTextureUVIndexBufferCount(void) const
    {
        return (m_TexUVBuffer.size());
    }
    inline size_t getVertexNormalIndexBufferCount(void) const
    {
        return (m_NormalBuffer.size());
    }
    inline size_t getEntitiesCount(void) const
    {
        return (m_totalEntitiesCount);
    }

    inline const VertexBuffer_t& getVertexBuffer(void) const
    {
        return (m_VBuffer);
    }

    inline bool isEmpty(void) const
    {
        return (m_facesTable.size() == 0);
    }

private:

    /**
      *  @brief  Register pointers to Obj entities into the entity table.
      *  @details This function should be called after all entities have been created and stored
      *           to get valid pointers (std containers' resizing invalidates pointers and
      *           iterators).
      */
    void sync(void);

    // Members =====================================================================================

    EntityRefList_t m_entityTable;                         //< Queue of pointers to all the entities.
    mutable EntityRefList_t::iterator m_lastFetchedEntity; //< Last fetched entity.

    FacesBuffer_t m_facesTable;                           //< Vector of Face entities.

    std::vector<ObjGroup> m_groupTable;                  //< Vector of Groups.
    std::array<int64_t, 3> m_currentGroupsIdx = {-1, -1, -1};    //< Array of current Groups.
    mutable std::vector<ObjGroup>::iterator m_lastFetchedGroup;  //< Last fetched Group.

    VertexBuffer_t m_VBuffer;          //< Vertex buffer.
    TextureUVBuffer_t m_TexUVBuffer;   //< Vertex texture buffer.
    NormalBuffer_t m_NormalBuffer;     //< Vertex normal buffer.

    IndexBuffer_t m_IdxBuffer;         //< Index buffer.

    VerticesRefList_t m_lastReturnedVertices; //< List of vertices to be returned on request.

    size_t m_totalEntitiesCount = 0;   //< Total number of entities.
    bool m_DBSynced = false;           //< Was the entityTable synced?
};

#endif /* __OBJDATABASE_H__ */
