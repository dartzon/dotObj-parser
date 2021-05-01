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
#include "ObjEntityGroup.h"
#include "ObjEntity.h"
#include "ObjEntityFace.h"
#include "ObjDatabase.h"
#include "ObjFileParser.h"

#include <cstdio>

#include <algorithm>

#include <cstdio>

int main(int argc, char* argv[])
{
    namespace fs = std::filesystem;
    fs::path objFilePath(
        "/home/dartzon/Dev-Workspace/DotObj/tests/models/Aircraft_Carrier/Liaoning.obj");

    if (argc > 1)
    {
        objFilePath = argv[1];
    }

    const std::string fileName = objFilePath;

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
            "// Obj file: %s\n\n"
            "function renderObjFile()\n{\n"
            "var material = new THREE.MeshStandardMaterial( { color : 0x00cc00 } );\n"
            "var color_triangle = new THREE.Color( 0xffaa00 );\n"
            "var color_quad = new THREE.Color( 0xe21aee );\n"
            "\nvar materialIndex = 0;\n"
            "\nvar geometry = new THREE.Geometry();\n"
            "geometry.vertices = [\n",
            fileName.c_str());

    // =================================================================================================
    // Create Vertices buffers.
    // =================================================================================================

    std::string vertexBuffer;
    vertexBuffer.reserve(objDB.getVerticesCount() * (30 + 7));
    std::for_each(cbegin<ElementType::VERTEX>(objDB),
                  cend<ElementType::VERTEX>(objDB),
                  [scale, &vertexBuffer](const Vertex_t& vtx) {
                      vertexBuffer += "\tnew THREE.Vector3(";
                      vertexBuffer += std::to_string(vtx.m_x * scale);
                      vertexBuffer += ',';
                      vertexBuffer += std::to_string(vtx.m_y * scale);
                      vertexBuffer += ',';
                      vertexBuffer += std::to_string(vtx.m_z * scale);
                      vertexBuffer += "),\n";
                  });

    vertexBuffer[vertexBuffer.size() - 2] = '\n';
    vertexBuffer.back() = ']';
    fprintf(smtObjFile.get(), "%s;\n", vertexBuffer.c_str());

    // =================================================================================================
    // Create Faces.
    // =================================================================================================

    std::string faceBuffer("geometry.faces = [\n");

    auto cycleVerticesType = [](const VerticesIdxOrganization vtxIdxOrg, ElementType& vtxType) {
        switch (vtxIdxOrg)
        {
        case VerticesIdxOrganization::VGEO_VNORMAL:
            vtxType = (vtxType == ElementType::VERTEX) ? ElementType::VERTEX_NORMAL :
                                                         ElementType::VERTEX;
            break;

        case VerticesIdxOrganization::VGEO_VTEXTURE:
            vtxType = (vtxType == ElementType::VERTEX) ? ElementType::VERTEX_TEXTURE :
                                                         ElementType::VERTEX;
            break;

        case VerticesIdxOrganization::VGEO_VTEXTURE_VNORMAL:
            if (vtxType == ElementType::VERTEX)
            {
                vtxType = ElementType::VERTEX_TEXTURE;
            }
            else if (vtxType == ElementType::VERTEX_TEXTURE)
            {
                vtxType = ElementType::VERTEX_NORMAL;
            }
            else
            {
                vtxType = ElementType::VERTEX;
            }
            break;

        case VerticesIdxOrganization::VGEO:
        default: break;
        }

        return vtxType;
    };

    std::vector<size_t> vtxBuffer;
    std::vector<size_t> normalBuffer;

    std::for_each(
        cbegin<ElementType::FACE>(objDB),
        cend<ElementType::FACE>(objDB),
        [&objDB, &faceBuffer, &vtxBuffer, &normalBuffer, &cycleVerticesType](
            const ObjEntityFace& face) {
            IndexBufferRangeIterators_t idxBufRefR = objDB.getVerticesIterators(face);
            ElementType vertexType = ElementType::VERTEX;

            std::for_each(idxBufRefR.first,
                          idxBufRefR.second,
                          [&face, &vtxBuffer, &normalBuffer, &vertexType, &cycleVerticesType](
                              const size_t idx) {
                              switch (vertexType)
                              {
                              case ElementType::VERTEX: vtxBuffer.push_back(idx - 1); break;

                              case ElementType::VERTEX_TEXTURE: break;

                              case ElementType::VERTEX_NORMAL:
                                  normalBuffer.push_back(idx - 1);
                                  break;

                              default: break;
                              }

                              vertexType = cycleVerticesType(face.getVerticesIndicesOrganization(),
                                                             vertexType);
                          });

            for (size_t vtxIdx = 0; vtxIdx < vtxBuffer.size() - 2; ++vtxIdx)
            {
                faceBuffer += "\tnew THREE.Face3(";
                faceBuffer += std::to_string(vtxBuffer[0]);
                faceBuffer += ',';
                faceBuffer += std::to_string(vtxBuffer[vtxIdx + 1]);
                faceBuffer += ',';
                faceBuffer += std::to_string(vtxBuffer[vtxIdx + 2]);
                faceBuffer += ',';

                if (normalBuffer.empty() == true)
                {
                    faceBuffer += " null, ";
                }
                else
                {
                    auto vtxNormal1 = objDB.getVertex(ElementType::VERTEX_NORMAL, normalBuffer[0]);
                    auto vtxNormal2 = objDB.getVertex(ElementType::VERTEX_NORMAL,
                                                      normalBuffer[vtxIdx + 1]);
                    auto vtxNormal3 = objDB.getVertex(ElementType::VERTEX_NORMAL,
                                                      normalBuffer[vtxIdx + 2]);

                    faceBuffer += "[\n";

                    {
                        auto [x, y, z, w] = (*vtxNormal1).get();
                        faceBuffer += " new THREE.Vector3(";
                        faceBuffer += std::to_string(x);
                        faceBuffer += ',';
                        faceBuffer += std::to_string(y);
                        faceBuffer += ',';
                        faceBuffer += std::to_string(z);
                        faceBuffer += ')';
                        faceBuffer += ',';
                    }

                    {
                        auto [x, y, z, w] = (*vtxNormal2).get();
                        faceBuffer += " new THREE.Vector3(";
                        faceBuffer += std::to_string(x);
                        faceBuffer += ',';
                        faceBuffer += std::to_string(y);
                        faceBuffer += ',';
                        faceBuffer += std::to_string(z);
                        faceBuffer += ')';
                        faceBuffer += ',';
                    }

                    {
                        auto [x, y, z, w] = (*vtxNormal3).get();
                        faceBuffer += " new THREE.Vector3(";
                        faceBuffer += std::to_string(x);
                        faceBuffer += ',';
                        faceBuffer += std::to_string(y);
                        faceBuffer += ',';
                        faceBuffer += std::to_string(z);
                        faceBuffer += ')';
                    }

                    faceBuffer += ']';
                    faceBuffer += ',';
                }

                faceBuffer += " color_triangle, materialIndex),\n";
            }

            vtxBuffer.clear();
            normalBuffer.clear();
        });

    faceBuffer[faceBuffer.size() - 2] = '\n';
    faceBuffer.back() = ']';
    fprintf(smtObjFile.get(), "%s;\n/* geometry.computeFaceNormals(); */\n", faceBuffer.c_str());
    fprintf(smtObjFile.get(), "\n\nreturn geometry;\n}\n");

    return 0;
}
