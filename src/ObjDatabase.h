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

#include "ObjEntityVertex.h"
#include "ObjEntityFace.h"
#include "ObjEntityGroup.h"

#include <vector>
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

    /// \brief  Insert a new Obj entity.
    ///
    /// \param  obj Obj entity to be inserted.
    /// \return ID of the inserted entity in the entities buffer.
    template<typename EntT>
    size_t insertEntity(EntT&& obj)
    {
        constexpr bool isGroup = std::is_same_v<std::remove_reference_t<EntT>, ObjEntityGroup>;
        constexpr bool isVertex = std::is_same_v<std::remove_reference_t<EntT>, ObjEntityVertex>;
        constexpr bool isFace = std::is_same_v<std::remove_reference_t<EntT>, ObjEntityFace>;

        static_assert(std::is_base_of_v<ObjEntity, std::remove_reference_t<EntT>> == true,
                      "Only ObjEntities are allowed");

        std::vector<std::remove_reference_t<EntT>>* pBuffer = nullptr;

        size_t entityID = 0;

        if constexpr (isGroup == true)
        {
            entityID = obj.getID();

            // Do not create a new group if it already exists.
            if (getGroup(entityID).has_value() == false)
            {
                pBuffer = &m_groupBuffer;
                entityID = 0;
            }
        }
        else if constexpr (isVertex == true)
        {
            uint8_t bufferIdx = 0;

            switch (obj.getType())
            {
            case ElementType::VERTEX: bufferIdx = 0; break;
            case ElementType::VERTEX_TEXTURE: bufferIdx = 1; break;
            case ElementType::VERTEX_NORMAL: bufferIdx = 2; break;
            case ElementType::VERTEX_PARAM_SPACE: bufferIdx = 3; break;

            default: break;
            }

            pBuffer = &m_vertexBuffer[bufferIdx];
            entityID = pBuffer->size();
        }
        else if constexpr (isFace == true)
        {
            pBuffer = &m_faceBuffer;
        }

        if (pBuffer != nullptr)
        {
            if (entityID == 0)
            {
                entityID = m_allEntitiesTable.size() + 1;
            }
            obj.setID(entityID);
            pBuffer->push_back(std::forward<EntT>(obj));

            // Hold a reference to the newly created entity.
            m_allEntitiesTable.push_back(pBuffer->back());
        }

        return entityID;
    }

    /// \brief  Return the list of vertices that compose the given entity.
    ///
    /// \param  elemWithVertices Reference to an Obj entity.
    /// \return  Const reference to a vector of pointers to vertices.
    VerticesRefList_t getVerticesList(const VertexBasedEntity& elemWithVertices) const;

    /// \brief  Return a pair of iterators to the first and the last vertices indices.
    ///
    /// \param  elemWithVertices Reference to Obj entity.
    /// \return  pair of iterators to the first and the last vertices indices.
    IndexBufferRangeIterators_t
    getVerticesIterators(const VertexBasedEntity& elemWithVertices) const;

    /// \brief  Return a list entities included in an Obj Group.
    ///
    /// \param  group Concerned group.
    /// \return range of entites.
    EntitiesRefList_t getEntitiesInGroup(const ObjEntityGroup& group) const;

    /// \brief  Find a group by ID and return it.
    ///
    /// \param  id the group's ID.
    /// \return reference to the group.
    std::optional<std::reference_wrapper<ObjEntityGroup>> getGroup(const size_t id)
    {
        if (auto grpItr = std::find_if(m_groupBuffer.begin(),
                                       m_groupBuffer.end(),
                                       [id](const auto& grp) { return (grp.getID() == id); });
            grpItr != m_groupBuffer.end())
        {
            return *grpItr;
        }

        return std::nullopt;
    }

    /// \brief  Find a group by ID and return it.
    ///
    /// \param  id the group's ID.
    /// \return reference to the group.
    std::optional<std::reference_wrapper<const ObjEntityGroup>> getGroup(const size_t id) const
    {
        if (const auto grpItr = std::find_if(m_groupBuffer.cbegin(),
                                             m_groupBuffer.cend(),
                                             [id](const auto& grp) { return (grp.getID() == id); });
            grpItr != m_groupBuffer.cend())
        {
            return *grpItr;
        }

        return std::nullopt;
    }

    /// \brief  Insert a new vertex index.
    ///
    /// \param  idx the vertex's index to insert.
    void insertIndex(const size_t idx) { m_IdxBuffer.push_back(idx); }

    /// \brief  Return a vertex based on its type and index in the index buffer.
    ///
    /// \param  obj Obj entity to be inserted.
    std::optional<std::reference_wrapper<const ObjEntityVertex>> getVertex(const ElementType type,
                                                                           const size_t idx) const
    {
        switch (type)
        {
        case ElementType::VERTEX:
            if (idx < m_vertexBuffer[0].size())
            {
                return m_vertexBuffer[0][idx];
            }

        case ElementType::VERTEX_TEXTURE:
            if (idx < m_vertexBuffer[1].size())
            {
                return m_vertexBuffer[1][idx];
            }

        case ElementType::VERTEX_NORMAL:
            if (idx < m_vertexBuffer[2].size())
            {
                return m_vertexBuffer[2][idx];
            }

        default: break;
        }

        return std::nullopt;
    }

    /// \brief  Pre-allocate memory for the next wave of vertices indices.
    void reserveIndexBufferMemory()
    {
        size_t memSize = 0;
        for (const auto& vBuffer : m_vertexBuffer)
        {
            memSize += vBuffer.size();
        }

        m_IdxBuffer.reserve(memSize);
    }

    // Iterators functions
    // =========================================================================

    template<const ElementType type>
    constexpr auto begin() noexcept
    {
        return getBufferForType<type>().begin();
    }
    template<const ElementType type>
    constexpr auto begin() const noexcept
    {
        return getBufferForType<type>().begin();
    }
    template<const ElementType type>
    constexpr auto cbegin() const noexcept
    {
        return getBufferForType<type>().cbegin();
    }
    template<const ElementType type>
    constexpr auto end() noexcept
    {
        return getBufferForType<type>().end();
    }
    template<const ElementType type>
    constexpr auto end() const noexcept
    {
        return getBufferForType<type>().end();
    }
    template<const ElementType type>
    constexpr auto cend() const noexcept
    {
        return getBufferForType<type>().cend();
    }

    // Accessors
    // ===================================================================================

    size_t getGroupsCount() const { return m_groupBuffer.size(); }
    size_t getIndexBufferCount() const { return m_IdxBuffer.size(); }
    size_t getVerticesCount() const { return m_vertexBuffer[0].size(); }
    size_t getFacesCount() const { return m_faceBuffer.size(); }
    size_t getEntitiesCount() const { return m_allEntitiesTable.size(); }
    bool isEmpty() const { return m_allEntitiesTable.empty(); }

private:
    /// \brief  Get the approriate buffer for the provided element's type.
    ///
    /// \return  Entity buffer.
    template<const ElementType type>
    constexpr auto& getBufferForType()
    {
        if constexpr (type == ElementType::VERTEX)
        {
            return m_vertexBuffer[0];
        }
        else if constexpr (type == ElementType::VERTEX_TEXTURE)
        {
            return m_vertexBuffer[1];
        }
        else if constexpr (type == ElementType::VERTEX_NORMAL)
        {
            return m_vertexBuffer[2];
        }
        else if constexpr (type == ElementType::VERTEX_PARAM_SPACE)
        {
            return m_vertexBuffer[3];
        }
        else if constexpr (type == ElementType::FACE)
        {
            return m_faceBuffer;
        }
        else
        {
            return m_allEntitiesTable;
        }
    }

    /// \brief  Get the approriate buffer for the provided element's type.
    ///
    /// \return  Entity buffer.
    template<const ElementType type>
    constexpr const auto& getBufferForType() const
    {
        if constexpr (type == ElementType::VERTEX)
        {
            return m_vertexBuffer[0];
        }
        else if constexpr (type == ElementType::VERTEX_TEXTURE)
        {
            return m_vertexBuffer[1];
        }
        else if constexpr (type == ElementType::VERTEX_NORMAL)
        {
            return m_vertexBuffer[2];
        }
        else if constexpr (type == ElementType::VERTEX_PARAM_SPACE)
        {
            return m_vertexBuffer[3];
        }
        else if constexpr (type == ElementType::FACE)
        {
            return m_faceBuffer;
        }
        else
        {
            return m_allEntitiesTable;
        }
    }

    // Members
    // =====================================================================================

    IndexBuffer_t m_IdxBuffer;             ///< Index buffer.
    VertexBuffer_t m_vertexBuffer;         ///< Vertex buffer.
    FaceBuffer_t m_faceBuffer;             ///< Map of Faces.
    GroupBuffer_t m_groupBuffer;           ///< Map of Groups.
    EntitiesRefList_t m_allEntitiesTable;  ///< Vector of references to all Obj entities.
};

// Iterators free functions
// ========================================================================

template<const ElementType type>
constexpr auto begin(ObjDatabase& db) noexcept
{
    return db.begin<type>();
}
template<const ElementType type>
constexpr auto begin(const ObjDatabase& db) noexcept
{
    return db.begin<type>();
}
template<const ElementType type>
constexpr auto cbegin(const ObjDatabase& db) noexcept
{
    return db.cbegin<type>();
}
template<const ElementType type>
constexpr auto end(ObjDatabase& db) noexcept
{
    return db.end<type>();
}
template<const ElementType type>
constexpr auto end(const ObjDatabase& db) noexcept
{
    return db.end<type>();
}
template<const ElementType type>
constexpr auto cend(const ObjDatabase& db) noexcept
{
    return db.cend<type>();
}

#endif /* OBJDATABASE_H_ */
