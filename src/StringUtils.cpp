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

/// \file      StringUtils.cpp
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      08-11-2017

#include "Utils.h"
#include "Types.h"

#include <algorithm>

namespace ObjUtils
{
void StringUtils::removeSurroundingBlanks(std::string& str)
{
    // Look for the first non blank character.
    CStringIterator_t strItr = std::find_if_not(str.cbegin(), str.cend(), &isblank);
    if (strItr != str.end())
    {
        str.erase(str.cbegin(), strItr);
    }

    // Look for the first non blank character starting from the end of the string.
    RStringIterator_t strRItr = std::find_if_not(str.rbegin(), str.rend(), &isspace);
    if (strRItr != str.rend())
    {
        // Resize the string to remove blanks.
        const size_t sz = std::distance(str.rbegin(), strRItr);
        str.resize(str.size() - sz);
    }
}

} /* namespace ObjUtils */
