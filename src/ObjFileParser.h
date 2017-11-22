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
 * \file      ObjFileParser.h
 *
 * \brief     Parser for Wavefront Obj files.
 *
 * \author    Othmane AIT EL CADI - dartzon@gmail.com
 * \date      08-11-2017
 */

#ifndef __OBJFILEPARSER_H__
#define __OBJFILEPARSER_H__

#include "Types.h"
#include "ObjDatabase.h"

#include <memory>

class ObjFileParser
{
public:

    /**
      *  @brief  Constructor.
      *
      *  @param  pObjFilePath Obj file path.
      */
    ObjFileParser(const char* pObjFilePath) :
        m_pObjFilePath(pObjFilePath)
    {
    }

    /**
      *  @brief  Parse an Obj file.
      *
      *  @return  An Obj Database instance.
      */
    ObjDatabase parseFile(void);

private:

    /**
      *  @brief  Parse one line of the Obj file.
      *
      *  @param  oneLine Line to parse.
      */
    void parseLine(const std::string& oneLine);

    /**
      *  @brief  Parse Obj element's parameters.
      *
      *  @param  elementIDRes pair of the element type and the index to its first parameter.
      */
    void parseElementDataArgs(const ElemIDResult_t& elementIDRes);

    /**
      *  @brief  Get the current Obj element type.
      *
      *  @param  oneLine Line to parse.
      *  @return  pair of the element type and the index to its first parameter.
      */
    ElemIDResult_t getElementType(const std::string& oneLine);

    /**
      *  @brief  Get the vertices indices' organization for elements with indices.
      *
      *  @param  oneLine Line to parse.
      *  @return  Vertices indices organization.
      */
    VerticesIdxOrganization getIndicesOrganization(const std::string& args);

    /**
      *  @brief  Parse Vertex data.
      *
      *  @param  elementIDRes pair of the element type and the index to its first parameter.
      */
    void parseVertex(const ElemIDResult_t& elementIDRes);

    /**
      *  @brief  Parse Face data.
      *
      *  @param  elementIDRes pair of the element type and the index to its first parameter.
      */
    void parseFace(const ElemIDResult_t& elementIDRes);

    /**
      *  @brief  Parse Group data.
      *
      *  @param  elementIDRes pair of the element type and the index to its first parameter.
      */
    void parseGroup(const ElemIDResult_t& elementIDRes);

    // List of all Obj file keywords.
    const KeywordsArray_t elementsKeywords = {"v", "vt", "vn", "vp",
                                              "deg", "bmat", "step", "cstype",
                                              "p", "l", "f", "curv", "curv2", "surf",
                                              "parm", "trim", "hole", "scrv", "sp", "end",
                                              "con",
                                              "g", "s", "mg", "o",
                                              "bevel", "c_interp", "d_interp", "lod",
                                              "usemtl", "mtllib", "shadow_obj", "trace_obj",
                                              "ctech", "stech"};

    const char* m_pObjFilePath;
    ObjDatabase m_objDB;
    ElementType m_lastElementType = ElementType::NONE;
};

#endif /* __OBJFILEPARSER_H__ */
