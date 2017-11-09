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
 * \file      ObjGroup.h
 *
 * \brief     Group of Obj elements.
 * \details   Representation of a group of Obj elements.
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __OBJGROUP_H__
#define __OBJGROUP_H__

#include "Types.h"

class ObjGroup
{
public:

    /**
      *  @brief  Constructor for (g/o).
      *
      *  @param  groupName Name of the group.
      *  @param  entityTableIdx Index of the first included entity.
      *  @param  eGroupType Group type.
      */
    ObjGroup(std::string&& groupName, const size_t entityTableIdx, const ElementType eGroupType) :
        m_groupName(std::move(groupName)), m_entityTableIdx(entityTableIdx),
        m_resolution(0), m_eGroupType(eGroupType)
    {
    }

    /**
      *  @brief  Constructor for (s/mg).
      *
      *  @param  groupNum Number of the group.
      *  @param  entityTableIdx Index of the first included entity.
      *  @param  resolution Resolution of the group if (mg).
      *  @param  eGroupType Group type.
      */
    ObjGroup(const size_t groupNum, const size_t entityTableIdx, const uint32_t resolution,
             const ElementType eGroupType) :
        m_groupNum(groupNum), m_entityTableIdx(entityTableIdx),
        m_resolution(resolution), m_eGroupType(eGroupType)
    {
    }

    ObjGroup(const ObjGroup&) = default;
    ObjGroup(ObjGroup&& refGrp) = default;

    // Operators ===================================================================================

    ObjGroup& operator=(const ObjGroup&) = default;
    ObjGroup& operator=(ObjGroup&& refGrp) = default;

    /**
      *  @brief  Include one more entity in this group.
      */
    inline ObjGroup& operator++(void)
    {
        ++m_entityTableOffset;
        return (*this);
    }

    // =============================================================================================

    /**
      *  @brief  Returns a pair representing a range of entities indices.
      *
      *  @return Pair of entities indices [start, end].
      */
    EntitiesIndexRange_t getEntitiesIndicesRange(void) const
    {
        return (std::make_pair(m_entityTableIdx, m_entityTableOffset));
    }

    // Accessors ===================================================================================

    inline const std::string& getGroupName(void) const
    {
        return (m_groupName);
    }
    inline size_t getGroupNumber(void) const
    {
        return (m_groupNum);
    }
    inline size_t getFirstIncludedEntityIndex(void) const
    {
        return (m_entityTableIdx);
    }
    inline size_t getIncludedEntitiesCount(void) const
    {
        return (m_entityTableOffset);
    }
    inline uint32_t getResolution(void) const
    {
        return (m_resolution);
    }
    inline ElementType getType(void) const
    {
        return (m_eGroupType);
    }

private:

    // Members =====================================================================================

    std::string m_groupName;         //< Group name (g/o).
    size_t m_groupNum = 0;           //< Groupe number (s/mg).
    size_t m_entityTableIdx;         //< Index of the first entity included in this group.
    size_t m_entityTableOffset = 0;  //< Count of entities included in this group.
    uint32_t m_resolution;           //< Resolution of the Merging group (mg).
    ElementType m_eGroupType;        //< Group type.
};

#endif /* __OBJGROUP_H__ */
