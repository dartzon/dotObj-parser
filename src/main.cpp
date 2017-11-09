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
 * \file      main.cpp
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      04-11-2017
 */

#include "Utils.h"
#include "Types.h"
#include "ObjGroup.h"
#include "ObjEntity.h"
#include "WithVerticesIndices.h"
#include "ObjEntityFace.h"
#include "ObjDatabase.h"
#include "ObjFileParser.h"

#include <algorithm>

int main(int argc, char *argv[])
{
    const char* objFilePath = "";

    if(argc > 1)
    {
        objFilePath = argv[1];
    }

    ObjDatabase gObjDB;
    ObjFileParser fp(objFilePath, gObjDB);
    if(fp.parseFile() == false)
    {
        return (1);
    }

    if(gObjDB.getGroupsCount() > 0)
    {
        ObjGroup* pGrp = nullptr;
        while((pGrp = gObjDB.getNextGroup()) != nullptr)
        {
            printf(">>> Group:\n");

            EntitiesRefRange_t rng = gObjDB.getEntitiesInGroup(*pGrp);
            std::for_each(rng.first, rng.second,
                          [&gObjDB](const ObjEntity* pEnt)
                          {
                              if(pEnt->getType() == ElementType::FACE)
                              {
                                  const ObjEntityFace& fc = *static_cast<const ObjEntityFace*>(pEnt);

                                  VerticesRefList_t vtxRefLst = gObjDB.getVerticesList(fc);

                printf(">>> f [%s]\n", (fc.isTriangle() == true) ? "TRIANGLE" : "QUAD");
                std::for_each(vtxRefLst.cbegin(), vtxRefLst.cend(),
                              [](const Vertex_t* pVtx)
                              {
                                  printf("%f %f %f\n", pVtx->m_x, pVtx->m_y, pVtx->m_z);
                              });
                              }
                          });
        }
    }
    else
    {
        ObjEntity* pEnt = nullptr;
        while((pEnt = gObjDB.getNextEntity()) != nullptr)
        {
            if(pEnt->getType() == ElementType::FACE)
            {
                ObjEntityFace& fc = *static_cast<ObjEntityFace*>(pEnt);

                VerticesRefList_t vtxRefLst = gObjDB.getVerticesList(fc);

                printf(">>> f [%s]\n", (fc.isTriangle() == true) ? "TRIANGLE" : "QUAD");
                std::for_each(vtxRefLst.cbegin(), vtxRefLst.cend(),
                              [](const Vertex_t* pVtx)
                              {
                                  printf("%f %f %f\n", pVtx->m_x, pVtx->m_y, pVtx->m_z);
                              });
            }
        }
    }

    return (0);
}
