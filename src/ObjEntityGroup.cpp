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

/// \file      ObjEntityGroup.cpp
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      11-04-2019

// Local includes.
#include "ObjEntityGroup.h"

ObjEntityGroup::ObjEntityGroup(const ElementType eGroupType,
                               const size_t entityTableIdx,
                               std::string_view groupName) :
    ObjEntity(eGroupType),
    m_eGroupType(eGroupType), m_entityTableIdx(entityTableIdx),
    m_nameOrNumberID(std::string{groupName})
{
    startIncludedEntityRange(entityTableIdx);

    if (eGroupType == ElementType::OBJECT_NAME)
    {
        // Object name groups designate one and only one entity.
        m_entityTableOffset = 1;

        endIncludedEntityRange(entityTableIdx + 1);
    }
}

// =================================================================================================

ObjEntityGroup::ObjEntityGroup(const ElementType eGroupType,
                               const size_t entityTableIdx,
                               const size_t groupNum) :
    ObjEntity(eGroupType),
    m_eGroupType(eGroupType), m_entityTableIdx(entityTableIdx), m_nameOrNumberID(groupNum)
{
    startIncludedEntityRange(entityTableIdx);
}

// =================================================================================================

ObjEntityGroup::ObjEntityGroup(const ElementType eGroupType,
                               const size_t entityTableIdx,
                               const size_t groupNum,
                               const uint32_t resolution) :
    ObjEntity(eGroupType),
    m_eGroupType(eGroupType), m_entityTableIdx(entityTableIdx),
    m_nameOrNumberID(MergingGroupData{groupNum, resolution})

{
    startIncludedEntityRange(entityTableIdx);
}

// =================================================================================================

ObjEntityGroup::ObjEntityGroup(ObjEntityGroup&& refGrp) noexcept :
    ObjEntity(std::move(refGrp)), m_eGroupType(refGrp.m_eGroupType),
    m_entityTableIdx(refGrp.m_entityTableIdx), m_nameOrNumberID(std::move(refGrp.m_nameOrNumberID)),
    m_entityTableOffset(refGrp.m_entityTableOffset),
    m_includedEntities(std::move(refGrp.m_includedEntities))
{
}

// =================================================================================================

bool ObjEntityGroup::operator==(const ObjEntity& other) const
{
    const ObjEntityGroup& otherGrp = static_cast<const ObjEntityGroup&>(other);

    bool equals = false;

    if (m_eGroupType == otherGrp.m_eGroupType)
    {
        switch (m_eGroupType)
        {
        case ElementType::GROUP_NAME:
        case ElementType::OBJECT_NAME:
        {
            // Groups' names comparison.
            const std::string& thisGrpName = std::get<0>(m_nameOrNumberID);
            const std::string& otherGrpName = std::get<0>(otherGrp.m_nameOrNumberID);

            equals = (thisGrpName == otherGrpName);
        }
        break;

        case ElementType::SMOOTHING_GROUP:
        case ElementType::MERGING_GROUP:
        {
            // Groups' numbers comparison.
            const auto thisGrpNum = getGroupNumber();
            const auto otherGrpNum = otherGrp.getGroupNumber();

            equals = (thisGrpNum.value() == otherGrpNum.value());
        }
        break;

        default: break;
        }
    }

    return equals;
}

// =================================================================================================

auto ObjEntityGroup::getGroupName() const
{
    using ConstStringRef_t = std::reference_wrapper<const std::string>;

    return ((m_eGroupType == ElementType::GROUP_NAME) ||
            (m_eGroupType == ElementType::OBJECT_NAME)) ?
               std::optional<ConstStringRef_t>{std::get<0>(m_nameOrNumberID)} :
               std::nullopt;
}

// =================================================================================================

std::optional<size_t> ObjEntityGroup::getGroupNumber() const
{
    switch (m_eGroupType)
    {
    case ElementType::SMOOTHING_GROUP: return std::get<1>(m_nameOrNumberID);

    case ElementType::MERGING_GROUP:
    {
        const MergingGroupData& mgd = std::get<2>(m_nameOrNumberID);
        return std::optional<size_t>{mgd.m_grpNumber};
    }

    default: return std::nullopt;
    }
}

// =================================================================================================

std::optional<uint32_t> ObjEntityGroup::getResolution() const
{
    if (m_eGroupType == ElementType::MERGING_GROUP)
    {
        const MergingGroupData& mgd = std::get<2>(m_nameOrNumberID);
        return std::optional<size_t>{mgd.m_resolution};
    }

    return std::nullopt;
}
