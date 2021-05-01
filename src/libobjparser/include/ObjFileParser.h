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
#include <filesystem>
#include <optional>

/// \brief Parser for Wavefront Obj files.
class ObjFileParser
{
public:
    /// \brief  Constructor.
    ///
    /// \param  pObjFilePath Obj file path.
    ObjFileParser(const std::string& objFilePath) : m_objFilePath(objFilePath) {}

    /// \brief  Constructor.
    ///
    /// \param  pObjFilePath Obj file path.
    ObjFileParser(std::filesystem::path& objFilePath) : m_objFilePath(std::move(objFilePath)) {}

    /// \brief  Parse an Obj file.
    ///
    /// \return  An Obj Database instance.
    ObjDatabase parseFile();

private:
    /// \brief  Parse one line of the Obj file.
    ///
    /// \param  oneLine Line to parse.
    void parseElement(std::string_view oneLine);

    /// \brief  Parse Obj element's parameters.
    ///
    /// \param  elementIDRes pair of the element type and the index to its first parameter.
    void parseElementArgs(const ElemIDResult_t& elementIDRes);

    /// \brief  Get the current Obj element type.
    ///
    /// \param  oneLine Line to parse.
    /// \return  pair of the element type and the index to its first parameter.
    std::optional<ElemIDResult_t> getElementType(std::string_view oneLine);

    /// \brief  Get the vertices indices' organization for elements with indices.
    ///
    /// \param  oneLine Line to parse.
    /// \return  Vertices indices organization.
    std::optional<VerticesIdxOrganization> getIndicesOrganization(const std::string_view& args);

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

    /// \brief  Set the last included entity index for all the current groups.
    void endCurrentGroupsEntitiesRanges();

    // Members =====================================================================================

    /// Map of all Obj file keywords.
    const KeywordsMap_t elementsKeywords =
        {{"v", ElementType::VERTEX},
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
         {"fo", ElementType::FACE},  // As of version 2.11 f == fo (face outline).
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

    std::vector<size_t> m_currentGroups;  ///< The current active groups.

    const std::filesystem::path m_objFilePath;  ///< Path to the Obj file.
    ObjDatabase m_objDB;                        ///< Obj entities database.

    // It is safe to assume that the very first read element will be a Vertex, so initialize to
    // ElementType::VERTEX. (TODO: Fix this comment, vertex is not the 1st read element).
    ElementType m_lastElementType = ElementType::VERTEX;
};

#endif /* OBJFILEPARSER_H_ */
