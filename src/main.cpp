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

/// \file      main.cpp
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      04-11-2017

#include "Utils.h"
#include "Types.h"
#include "ObjGroup.h"
#include "ObjEntity.h"
#include "WithVerticesIndices.h"
#include "ObjEntityFace.h"
#include "ObjDatabase.h"
#include "ObjFileParser.h"

#include <cstdio>

#include <algorithm>

int main(int argc, char* argv[])
{
    const char* objFilePath = "";
    if (argc > 1)
    {
        objFilePath = argv[1];
    }

    float scale = 1.0f;
    if (argc > 2)
    {
        scale = std::stof(argv[2]);
    }

    ObjFileParser fp(objFilePath);
    const ObjDatabase objDB = fp.parseFile();
    if (objDB.isEmpty() == true)
    {
        return 1;
    }

    const char* pOutJSFile = "out3d/js/renderObjFile.js";
    const std::unique_ptr<std::FILE, decltype(&fclose)> smtObjFile(fopen(pOutJSFile, "w"), &fclose);

    fprintf(smtObjFile.get(),
            "function renderObjFile()\n{\n"
            "var geometry = new THREE.Geometry();\n"
            "var normal = null;\nvar color = new THREE.Color( 0xffaa00 );"
            "\nvar materialIndex = 0;\nvar face;\n");

    std::for_each(
        vertexCBegin(objDB), vertexCEnd(objDB), [&smtObjFile, scale](const Vertex_t& vtx) {
            const char* cmd = "geometry.vertices.push( new THREE.Vector3("
                              "%f, %f, %f) );\n";

            fprintf(smtObjFile.get(), cmd, vtx.m_x * scale, vtx.m_y * scale, vtx.m_z * scale);
        });

    for (const ObjEntityFace& fc : objDB)
    {
        IndexBufferRefRange_t idxBufRefR = objDB.getVerticesIndicesList(fc);

        const VerticesIdxOrganization vtxIdxOrg = fc.getVerticesIndicesOrganization();
        uint8_t step = 1;
        switch (vtxIdxOrg)
        {
        case VerticesIdxOrganization::VGEO: step = 1; break;

        case VerticesIdxOrganization::VGEO_VNORMAL:
        case VerticesIdxOrganization::VGEO_VTEXTURE: step = 2; break;

        case VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL: step = 3; break;

        default: break;
        }

        if (fc.isTriangle() == true)
        {
            const char* faceCmd = "face = new THREE.Face3( %lu, %lu, %lu, normal, color, "
                                  "materialIndex );\n"
                                  "geometry.faces.push( face );\n";

            fprintf(smtObjFile.get(),
                    faceCmd,
                    (*idxBufRefR.first) - 1,
                    *(idxBufRefR.first + step) - 1,
                    *(idxBufRefR.first + (step * 2)) - 1);
        }
        else
        {
            const char* faceCmd = "face = new THREE.Face3( %lu, %lu, %lu,"
                                  " normal, color, materialIndex );\n"
                                  "geometry.faces.push( face );\n";

            const size_t v1 = (*idxBufRefR.first) - 1;
            const size_t v2 = *(idxBufRefR.first + step) - 1;
            const size_t v3 = *(idxBufRefR.first + (step * 2)) - 1;
            const size_t v4 = *(idxBufRefR.first + (step * 3)) - 1;

            fprintf(smtObjFile.get(), faceCmd, v1, v2, v3);

            fprintf(smtObjFile.get(), faceCmd, v1, v3, v4);
        }
    }

    fprintf(smtObjFile.get(), "\n\nreturn geometry;\n}\n");

    return 0;
}
