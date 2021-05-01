/// MIT License
///
/// Copyright (c) 2017 Othmane AIT EL CADI
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

/// \file      MtlFileParser.h
///
/// \brief     Parser for Wavefront MTL files.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      21-05-2019

#ifndef MTLFILEPARSER_H_
#define MTLFILEPARSER_H_

#include "Types.h"
#include "ObjDatabase.h"

class MtlFileParser
{
public:
private:
    // Members =====================================================================================

    /// Map of all Obj file keywords.
    const KeywordsMap_t elementsKeywords =
        {{"Ka", ElementType::VERTEX},
         {"Kd", ElementType::VERTEX_TEXTURE},
         {"Ks", ElementType::VERTEX_NORMAL},
         {"Tf", ElementType::VERTEX_PARAM_SPACE},
         {"illum", ElementType::DEGREE},
         {"d", ElementType::BASIC_MATRIX},
         {"Ns", ElementType::STEP_SIZE},
         {"sharpness", ElementType::PARAM_CURVE_SURFACE},
         {"Ni", ElementType::POINT},
         {"map_Ka", ElementType::LINE},
         {"map_Kd", ElementType::FACE},
         {"map_Ks", ElementType::FACE},  // As of version 2.11 f == fo (face outline).
         {"map_Ns", ElementType::CURVE},
         {"map_d", ElementType::CURVE2D},
         {"disp", ElementType::SURFACE},
         {"decal", ElementType::PARAM_VAL},
         {"bump", ElementType::OUT_LOOP},
         {"refl", ElementType::IN_LOOP}};

    std::vector<size_t> m_currentGroups;  ///< The current active groups.

    const std::filesystem::path m_mtlFilePath = "";  ///< Path to the Obj file.
    ObjDatabase m_objDB;                             ///< Obj entities database.
};

#endif /* MTLFILEPARSER_H_ */
