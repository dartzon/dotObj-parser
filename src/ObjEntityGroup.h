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

/// \file      ObjEntityGroup.h
///
/// \brief     Group of Obj elements. Representation of a group of Obj elements.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#ifndef OBJENTITYGROUP_H_
#define OBJENTITYGROUP_H_

#include "Types.h"

#include "ObjEntity.h"

#include <variant>
#include <functional>

/// \brief Group of Obj elements.
class ObjEntityGroup : public ObjEntity
{
public:
    /// \brief  Constructor for types (g/o).
    ///
    /// \param  eGroupType Group type.
    /// \param  entityTableIdx Index of the first included entity.
    /// \param  groupName Name of the group.
    ObjEntityGroup(const ElementType eGroupType,
                   const size_t entityTableIdx,
                   std::string_view groupName);

    /// \brief  Constructor for type (s).
    ///
    /// \param  eGroupType Group type.
    /// \param  entityTableIdx Index of the first included entity.
    /// \param  groupNum Number of the group.
    ObjEntityGroup(const ElementType eGroupType,
                   const size_t entityTableIdx,
                   const size_t groupNum);

    /// \brief  Constructor for type (mg).
    ///
    /// \param  eGroupType Group type.
    /// \param  entityTableIdx Index of the first included entity.
    /// \param  groupNum Number of the group.
    /// \param  resolution Resolution of the group if (mg).
    ObjEntityGroup(const ElementType eGroupType,
                   const size_t entityTableIdx,
                   const size_t groupNum,
                   const uint32_t resolution);

    /// \brief  Default copy constructor.
    ObjEntityGroup(const ObjEntityGroup& refGrp) = default;

    /// \brief  Move constructor.
    ObjEntityGroup(ObjEntityGroup&& refGrp) noexcept;

    void startIncludedEntityRange(const size_t idx)
    {
        EntitiesIndexRange_t idxRange = std::make_pair(idx, 0);
        m_includedEntities.push_back(std::move(idxRange));
    }

    void endIncludedEntityRange(const size_t idx)
    {
        EntitiesIndexRange_t& idxRange = m_includedEntities.back();
        idxRange.second = idx;
    }

    // Operators
    // ===================================================================================

    /// \brief  Equality operator. Compares groups' types and names/numbers.
    bool operator==(const ObjEntity& other) const override;

    // Accessors ===================================================================================

    /// \brief  Return the group's name if type is (g/o).
    ///
    /// \return group's name or std::nullopt.
    auto getGroupName() const;

    /// \brief  Return the group's number if type is (s/mg).
    ///
    /// \return group's number or std::nullopt.
    std::optional<size_t> getGroupNumber() const;

    /// \brief  Return the group's merging resolution if type is (mg).
    ///
    /// \return group's resolution or std::nullopt.
    std::optional<uint32_t> getResolution() const;

    /// \brief  Return a pair of indexes representing the range of the included entities.
    ///
    /// \return Pair of entities indices [start, end].
    const std::vector<EntitiesIndexRange_t>& getEntitiesIndicesRange() const
    {
        return m_includedEntities;
    }

    size_t getIncludedEntitiesCount() const { return m_entityTableOffset; }

    size_t getFirstIncludedEntityIndex() const { return m_entityTableIdx; }

    size_t getIncludedEntityRangesCount() const { return m_includedEntities.size(); }

    const std::optional<std::reference_wrapper<const EntitiesIndexRange_t>>
    getIncludedEntityRange(const size_t idx) const
    {
        if (idx <= m_includedEntities.size())
        {
            return m_includedEntities[idx];
        }

        return std::nullopt;
    }

    const auto& getAllIncludedEntitiesRanges() const { return m_includedEntities; }

    const std::optional<size_t> getEntityRangeSize(const size_t idx) const
    {
        if (idx <= m_includedEntities.size())
        {
            const auto [start, end] = m_includedEntities[idx];
            return end - start + 1;
        }

        return std::nullopt;
    }

    const size_t getTotalEntitiesRangesSize() const
    {
        size_t count = 0;

        for (const auto [start, end] : m_includedEntities)
        {
            count += end - start + 1;
        }

        return count;
    }

private:
    // Members
    // =====================================================================================

    struct MergingGroupData
    {
        MergingGroupData(const size_t nbrID, const uint32_t res) :
            m_grpNumber(nbrID), m_resolution(res)
        {
        }

        const size_t m_grpNumber;     ///< Group number.
        const uint32_t m_resolution;  ///< Max distance between two surfaces that will be merged.
    };

    using NameOrNumberUnion_t = std::variant<std::string, size_t, MergingGroupData>;

    ElementType m_eGroupType;              ///< Group type.
    size_t m_entityTableIdx;               ///< Index of the first entity included in this group.
    NameOrNumberUnion_t m_nameOrNumberID;  ///< Group name or group number.
    size_t m_entityTableOffset = 0;        ///< Count of entities included in this group.
    std::vector<EntitiesIndexRange_t> m_includedEntities;  ///< Ranges of included entities.
};

// Typedefs
// ========================================================================================
using GroupBuffer_t = std::vector<ObjEntityGroup>;

#endif /* OBJENTITYGROUP_H_ */
