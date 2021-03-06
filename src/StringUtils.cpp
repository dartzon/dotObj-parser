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
#include <functional>

namespace ObjUtils
{
std::string_view StringUtils::removeSurroundingBlanks(const std::string& str)
{
    std::string_view noBlanksStr(str);

    // Look for the first non blank character.
    const auto firstBlankItr = std::find_if_not(noBlanksStr.cbegin(), noBlanksStr.cend(), &isblank);
    if (firstBlankItr != noBlanksStr.cend())
    {
        noBlanksStr.remove_prefix(std::distance(noBlanksStr.cbegin(), firstBlankItr));
    }

    // Look for the first non blank character starting from the end of the string.
    auto lastBlankItr = std::find_if_not(noBlanksStr.crbegin(), noBlanksStr.crend(), &isspace);
    if (lastBlankItr != noBlanksStr.crend())
    {
        noBlanksStr.remove_suffix(std::distance(noBlanksStr.crbegin(), lastBlankItr));
    }

    return noBlanksStr;
}

// =================================================================================================

std::string_view& StringUtils::removeSurroundingBlanks(std::string_view& str)
{
    // Look for the first non blank character.
    const auto firstBlankItr = std::find_if_not(str.cbegin(), str.cend(), &isblank);
    if (firstBlankItr != str.cend())
    {
        str.remove_prefix(std::distance(str.cbegin(), firstBlankItr));
    }

    // Look for the first non blank character starting from the end of the string.
    auto lastBlankItr = std::find_if_not(str.crbegin(), str.crend(), &isspace);
    if (lastBlankItr != str.crend())
    {
        str.remove_suffix(std::distance(str.crbegin(), lastBlankItr));
    }

    return str;
}

// =================================================================================================

std::vector<std::string_view>
StringUtils::splitString(const std::string& str, const std::initializer_list<const char> delimiters)
{
    std::function delimiterCheck(&isblank);
    if (delimiters.size() > 0)
    {
        delimiterCheck = [&delimiters](const auto chr) {
            return (isblank(chr) != 0) ||
                   (std::find(delimiters.begin(), delimiters.end(), chr) != delimiters.end());
        };
    }

    auto startItr = str.cbegin();
    auto splitItr = startItr;
    std::vector<std::string_view> subStrings;

    while ((splitItr = std::find_if(startItr, str.cend(), delimiterCheck)) != str.cend())
    {
        if (delimiterCheck(*startItr) == false)
        {
            subStrings.push_back(std::string_view(&*startItr, std::distance(startItr, splitItr)));
        }
        startItr = splitItr + 1;
    }

    if (startItr != str.cend())
    {
        subStrings.push_back(std::string_view(&*startItr, std::distance(startItr, str.cend())));
    }

    return subStrings;
}

// =================================================================================================

std::vector<std::string_view>
StringUtils::splitString(std::string_view str, const std::initializer_list<const char> delimiters)
{
    std::function delimiterCheck(&isblank);
    if (delimiters.size() > 0)
    {
        delimiterCheck = [&delimiters](const auto chr) {
            return (isblank(chr) != 0) ||
                   (std::find(delimiters.begin(), delimiters.end(), chr) != delimiters.end());
        };
    }

    auto startItr = str.cbegin();
    auto splitItr = startItr;
    std::vector<std::string_view> subStrings;

    while ((splitItr = std::find_if(startItr, str.cend(), delimiterCheck)) != str.cend())
    {
        if (delimiterCheck(*startItr) == false)
        {
            subStrings.push_back(std::string_view(&*startItr, std::distance(startItr, splitItr)));
        }
        startItr = splitItr + 1;
    }

    if (startItr != str.cend())
    {
        subStrings.push_back(std::string_view(&*startItr, std::distance(startItr, str.cend())));
    }

    return subStrings;
}

} /* namespace ObjUtils */
