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

/// \file      ObjEntity.h
///
/// \brief     Base class for Obj entities.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#ifndef OBJENTITY_H_
#define OBJENTITY_H_

#include "Types.h"

// Forward declaration of friend class.
class ObjDatabase;

/// \brief Base class for Obj entities.
class ObjEntity
{
public:
    ///  \brief  Constructor.
    ///
    ///  \param  entityType Type of the entity.
    explicit ObjEntity(const ElementType entityType) : m_ID(0), m_type(entityType) {}

    ///  \brief  Default move constructor.
    ObjEntity(ObjEntity&&) noexcept = default;

    // Accessors ===================================================================================

    size_t getID() const { return m_ID; }
    ElementType getType() const { return m_type; }

private:
    // Accessors ===================================================================================

    void setID(const size_t ID) { m_ID = ID; }

    // Members =====================================================================================

    size_t m_ID;         ///< Entity ID.
    ElementType m_type;  ///< Entity type.

    friend ObjDatabase;  ///< The ObjDatabase class will call ObjEntity::setID after each insertion.
};

#endif /* OBJENTITY_H_ */
