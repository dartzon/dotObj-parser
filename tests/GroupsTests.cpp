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
 * \file      GroupsTests.cpp
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      09-11-2017
 */

#include <algorithm>

#include "ObjDatabase.h"
#include "ObjFileParser.h"
#include "ObjEntityFace.h"

#include "catch.h"

TEST_CASE("Loading Groups", "[group]")
{
    const char* pFilePath = "tests/models/ducky.obj";
    ObjDatabase objDB;
    ObjFileParser fp(pFilePath, objDB);

    const bool opened = fp.parseFile();

    SECTION("reading and parsing the Obj file")
    {
        REQUIRE(opened == true);
    }
    SECTION("successful reading changes the count of objects in the Obj database")
    {
        const size_t grpCount = objDB.getGroupsCount();

        REQUIRE(grpCount > 0);
    }
    SECTION("there should be exactly 4 Groups in the Obj database")
    {
        size_t grpCount = 0;

        while(objDB.getNextGroup() != nullptr)
        {
            ++grpCount;
        }

        REQUIRE(grpCount == 4);
    }
    SECTION("all the groups are of type group name (g)")
    {
        bool isGrpName = true;

        ObjGroup* pGrp = nullptr;
        while((pGrp = objDB.getNextGroup()) != nullptr)
        {
            isGrpName &= (pGrp->getType() == ElementType::GROUP);
        }

        REQUIRE(isGrpName == true);
    }
}

