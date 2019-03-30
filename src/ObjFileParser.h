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

/// \file      ObjFileParser.h
///
/// \brief     Parser for Wavefront Obj files.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#ifndef OBJFILEPARSER_H_
#define OBJFILEPARSER_H_

#include "Types.h"
#include "ObjDatabase.h"

#include <memory>

/// \brief Parser for Wavefront Obj files.
class ObjFileParser
{
public:
    /// \brief  Constructor.
    ///
    /// \param  pObjFilePath Obj file path.
    ObjFileParser(const char* pObjFilePath) : m_pObjFilePath(pObjFilePath) {}

    /// \brief  Parse an Obj file.
    ///
    /// \return  An Obj Database instance.
    ObjDatabase parseFile();

private:
    /// \brief  Parse one line of the Obj file.
    ///
    /// \param  oneLine Line to parse.
    void parseLine(const std::string& oneLine);

    /// \brief  Parse Obj element's parameters.
    ///
    /// \param  elementIDRes pair of the element type and the index to its first parameter.
    void parseElementDataArgs(const ElemIDResult_t& elementIDRes);

    /// \brief  Get the current Obj element type.
    ///
    /// \param  oneLine Line to parse.
    /// \return  pair of the element type and the index to its first parameter.
    ElemIDResult_t getElementType(const std::string& oneLine);

    /// \brief  Get the vertices indices' organization for elements with indices.
    ///
    /// \param  oneLine Line to parse.
    /// \return  Vertices indices organization.
    VerticesIdxOrganization getIndicesOrganization(const std::string& args);

    /// \brief  Parse Vertex data.
    ///
    /// \param  elementIDRes pair of the element type and the index to its first parameter.
    void parseVertex(const ElemIDResult_t& elementIDRes);

    /// \brief  Parse Face data.
    ///
    /// \param  elementIDRes pair of the element type and the index to its first parameter.
    void parseFace(const ElemIDResult_t& elementIDRes);

    /// \brief  Parse Group data.
    ///
    /// \param  elementIDRes pair of the element type and the index to its first parameter.
    void parseGroup(const ElemIDResult_t& elementIDRes);

    // Members =====================================================================================

    /// Map of all Obj file keywords.
    const KeywordsMap_t elementsKeywords = {{"v", ElementType::VERTEX},
                                            {"vt", ElementType::VERTEX_TEXTURE},
                                            {"vn", ElementType::VERTEX_NORMAL},
                                            {"vp", ElementType::VERTEX_PARAM_SPACE},
                                            {"deg", ElementType::DEGREE},
                                            {"bmat", ElementType::BASIC_MATRIX},
                                            {"step", ElementType::STEP_SIZE},
                                            {"cstype", ElementType::PARAM_CURVE_SURFACE},
                                            {"p", ElementType::POINT},
                                            {"l", ElementType::LINE},
                                            {"f", ElementType::FACE},
                                            {"curv", ElementType::CURVE},
                                            {"curv2", ElementType::CURVE2D},
                                            {"surf", ElementType::SURFACE},
                                            {"parm", ElementType::PARAM_VAL},
                                            {"trim", ElementType::OUT_LOOP},
                                            {"hole", ElementType::IN_LOOP},
                                            {"scrv", ElementType::SPECIAL_CURVE},
                                            {"sp", ElementType::SPECIAL_POINT},
                                            {"end", ElementType::END_STAMTEMENT},
                                            {"con", ElementType::CONNECT},
                                            {"g", ElementType::GROUP_NAME},
                                            {"s", ElementType::SMOOTHING_GROUP},
                                            {"mg", ElementType::MERGING_GROUP},
                                            {"o", ElementType::OBJECT_NAME},
                                            {"bevel", ElementType::BEVEL_INTERPOL},
                                            {"c_interp", ElementType::COLOR_INTERPOL},
                                            {"d_interp", ElementType::DISSOLVE_INTERPOL},
                                            {"lod", ElementType::LOD},
                                            {"usemtl", ElementType::MATERIAL_NAME},
                                            {"mtllib", ElementType::MATERIAL_LIB},
                                            {"shadow_obj", ElementType::SHADOW_CASTING},
                                            {"trace_obj", ElementType::RAY_TRACING},
                                            {"ctech", ElementType::CURVE_APPROX_TECH},
                                            {"stech", ElementType::SURFACE_APPROX_TECH}};

    // Indices array of the current active groups [0] : g, [1] : o, [2] : s.
    // Each newly inserted entity will increment the corresponding group's entity count offset.
    std::array<int64_t, 3> m_currentGroupsIdx = {-1, -1, -1};

    const char* m_pObjFilePath;
    ObjDatabase m_objDB;

    // It is safe to assume that the very first read element will be a Vertex, so initialize to
    // ElementType::VERTEX.
    ElementType m_lastElementType = ElementType::VERTEX;
};

#endif /* OBJFILEPARSER_H_ */
