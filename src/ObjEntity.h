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
 * \file      ObjEntity.h
 *
 * \brief     Base class for Obj entities.
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __OBJENTITY_H__
#define __OBJENTITY_H__

#include "Types.h"

// Forward declaration of friend class.
class ObjDatabase;

class ObjEntity
{
public:
    ObjEntity(const ElementType entityType) :
        m_ID(0), m_type(entityType)
    {
    }

    // Accessors ===================================================================================

    size_t getID(void) const
    {
        return (m_ID);
    }
    ElementType getType(void) const
    {
        return (m_type);
    }

private:

    // Accessors ===================================================================================

    void setID(const size_t ID)
    {
        m_ID = ID;
    }

    // Members =====================================================================================

    size_t m_ID;         //< Entity ID.
    ElementType m_type;  //< Entity type.

    friend ObjDatabase;  //< The ObjDatabase class will call ObjEntity::setID after each insertion.
};

#endif /* __OBJENTITY_H__ */