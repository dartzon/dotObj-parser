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

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

struct Coordinates3D
{
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;
};

using Vertex = Coordinates3D;
using VertexTexture = Coordinates3D;
using VertexNormal = Coordinates3D;

using VertexBuffer = std::vector<Vertex>;
using TextureUVBuffer = std::vector<Vertex>;
using NormalBuffer = std::vector<Vertex>;
using IndexBuffer = std::vector<uint32_t>;

enum class ElementType
{
    NONE   = -1,
    VERTEX = 0,           //< v
    VERTEX_TEXTURE,       //< vt
    VERTEX_NORMAL,        //< vn
    VERTEX_PARAM_SPACE,   //< vp
    POINT = 8,            //< p
    LINE,                 //< l
    FACE                  //< f
};

// ------------------------------------------------------------------
//   GLOBALS
// ------------------------------------------------------------------
VertexBuffer gVBuffer;
TextureUVBuffer gTexUVBuffer;
NormalBuffer gNormalBuffer;

IndexBuffer gIdxBuffer;

const std::array<const char*, 35> elementsKeywords = {"v", "vt", "vn", "vp",
                                                     "deg", "bmat", "step", "cstype",
                                                     "p", "l", "f", "curv", "curv2", "surf",
                                                     "parm", "trim", "hole", "scrv", "sp", "end",
                                                     "con",
                                                     "g", "s", "mg", "o",
                                                     "bevel", "c_interp", "d_interp", "lod",
                                                     "usemtl", "mtllib", "shadow_obj", "trace_obj",
                                                     "ctech", "stech"};

// ------------------------------------------------------------------

void parseVertex(const ElementType elemType, const char* argsList)
{
    Vertex v;

    char* strEnd;
    v.m_x = strtof(argsList, &strEnd);
    argsList = strEnd;
    v.m_y = strtof(argsList, &strEnd);

    // Texture vertices usually have only 2 coordinates.
    if(argsList != strEnd)
    {
        argsList = strEnd;
        v.m_z = strtof(argsList, &strEnd);
    }

    printf("%f %f %f\n", v.m_x, v.m_y, v.m_z);

    switch(elemType)
    {
    case ElementType::VERTEX : gVBuffer.push_back(v); break;
    case ElementType::VERTEX_TEXTURE : gTexUVBuffer.push_back(v); break;
    case ElementType::VERTEX_NORMAL : gNormalBuffer.push_back(v); break;

    default: break;
    }
}

void parseFace(const char* argsList)
{
    bool hasTextureVertex = false;
    bool hasVertexNormal = false;

    const std::string args(argsList);

    const size_t countSlashes = std::count(args.cbegin(), args.cend(), '/');
    hasTextureVertex = (countSlashes == 1);
    hasVertexNormal = (countSlashes == 2);

    const size_t doubleSlashesPos = args.find("//");
    hasTextureVertex = (doubleSlashesPos == std::string::npos);

    char* strEnd = nullptr;
}

using ElemIDResult = std::pair<ElementType, std::string::const_iterator>;
ElemIDResult getElementType(const std::string& oneLine)
{
    using StrItr = std::string::const_iterator;

    // Skip white spaces at the beginning of the line.
    StrItr lineStartItr = std::find_if(oneLine.cbegin(), oneLine.cend(),
                                           [](const char c)
                                           {
                                               return (isblank(c) == 0);
                                           });

    // Skip this line if it is empty or is a comment line.
    if((isspace(*lineStartItr) != 0) || (*lineStartItr == '#'))
    {
        return (std::make_pair(ElementType::NONE, oneLine.cend()));
    }

    // Go to the next white space.
    StrItr blankAfterTokenItr = std::find_if(lineStartItr, oneLine.cend(), &isblank);

    // Construct our element token.
    const std::string elementToken(lineStartItr, blankAfterTokenItr);

    // Find the element token in the keywords list.
    using KeywordsItr = std::array<const char*, 35>::const_iterator;
    KeywordsItr keyItr = std::find_if(elementsKeywords.cbegin(), elementsKeywords.cend(),
                                      [&elementToken](const char* keyword)
                                      {
                                          return (elementToken.compare(keyword) == 0);
                                      });
    if(keyItr == elementsKeywords.cend())
    {
        return (std::make_pair(ElementType::NONE, oneLine.cend()));
    }

    // Get the index of the identified element type and convert it to its enum correspondence.
    using DiffIdx = std::array<const char*, 35>::difference_type;
    const DiffIdx elementTypeIdx = std::distance(elementsKeywords.cbegin(), keyItr);

    return (std::make_pair(static_cast<ElementType>(elementTypeIdx), blankAfterTokenItr));
}

void parseElementDataArgs(const ElementType elemType, const char* argsList)
{
    switch(elemType)
    {
    case ElementType::VERTEX:
    case ElementType::VERTEX_TEXTURE:
    case ElementType::VERTEX_NORMAL:
    case ElementType::VERTEX_PARAM_SPACE:
        parseVertex(elemType, argsList);
        break;
    }
}

void parseLine(const std::string& oneLine)
{
    const ElemIDResult elemTypeRes = getElementType(oneLine);

    if(elemTypeRes.first != ElementType::NONE)
    {
        // Parse the arguments of each element.
        parseElementDataArgs(elemTypeRes.first, &*(elemTypeRes.second));
    }
}

int main(void)
{
    const char* objFilePath = "/home/dartzon/Downloads/StorecastTest/cube.obj";
    const std::unique_ptr<std::FILE, decltype(&fclose)> smtObjFile(fopen(objFilePath, "r"),
                                                                   &fclose);
    if(smtObjFile == nullptr)
    {
        printf("Obj file not found !!!\n");
        return (1);
    }

    const uint16_t lineBufferSize = 256;
    char lineBuffer[lineBufferSize];

    std::string oneLine;
    oneLine.reserve(lineBufferSize);

    while(fgets(lineBuffer, lineBufferSize, smtObjFile.get()) != nullptr)
    {
        oneLine = lineBuffer;
        parseLine(oneLine);
    }

    // printf(">>> %d\n", vxx);

    return (0);
}
