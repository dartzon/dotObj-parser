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

/// \file      Utils.h
///
/// \brief     Debug utilities.
/// \details   Custom assert/logging functions and string manipulation class.
///
/// \author    Othmane AIT EL CADI - <dartzon@gmail.com>
/// \date      07-11-2017

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string_view>

#ifndef OBJ_DEBUG

#define OBJASSERT(ignore, _ignore)                                                                 \
    do                                                                                             \
    {                                                                                              \
        ((void)0);                                                                                 \
    } while (false);
#define OBJLOG(...)                                                                                \
    do                                                                                             \
    {                                                                                              \
        ((void)0);                                                                                 \
    } while (false);

#else

#include <cstdio>
#include <cstdint>
#include <sstream>

#define OBJHALT() abort()

namespace ObjUtils
{
/// \brief  Halts the execution and displays a message if test is false.
///
/// \param  pExpression expression to evaluate.
/// \param  pCause message to display if test is false.
/// \param  pFilename source filename.
/// \param  linenum line of the call in the caller function.
inline void assertFunction(const char* pExpression,
                           const char* pCause,
                           const char* pFilename,
                           const uint32_t linenum)
{
    printf("ASSERTION FAILED!\n\tExpr : (%s)\n\tCause : %s\n\tFile : %s\n\tLine : %u\n",
           pExpression,
           pCause,
           pFilename,
           linenum);
}

inline void
doLog_VarArgs(const char* pFilename, const uint32_t linenum, std::ostringstream& strBuilder)
{
    printf("[DBG INFO] %s(%u): %s\n", pFilename, linenum, strBuilder.str().c_str());
}

template<typename T, typename... Args>
inline void doLog_VarArgs(const char* pFilename,
                          const uint32_t linenum,
                          std::ostringstream& strBuilder,
                          T firstArg,
                          const Args&... args)
{
    strBuilder << firstArg;
    doLog_VarArgs(pFilename, linenum, strBuilder, args...);
}

/// \brief  Displays debug information messages.
///
/// \param  pFilename source filename.
/// \param  linenum line of the call in the caller function.
/// \param  args variadic parameters.
template<typename... Args>
inline void logFunction(const char* pFilename, const uint32_t linenum, const Args&... args)
{
    std::ostringstream strBuilder;
    doLog_VarArgs(pFilename, linenum, strBuilder, args...);
}

} /* namespace ObjUtils */

#define OBJASSERT(exp, msg)                                                                        \
    do                                                                                             \
    {                                                                                              \
        if ((exp) == false)                                                                        \
        {                                                                                          \
            ObjUtils::assertFunction(#exp, msg, __FILE__, __LINE__);                               \
            OBJHALT();                                                                             \
        }                                                                                          \
    } while (false)

#define OBJLOG(...)                                                                                \
    do                                                                                             \
    {                                                                                              \
        ObjUtils::logFunction(__FILE__, __LINE__, __VA_ARGS__);                                    \
    } while (false)

#endif

#include <string>

namespace ObjUtils
{
/// \brief  String helper functions.
class StringUtils final
{
public:
    /// \brief  This class is not to be instanciated.
    StringUtils() = delete;

    /// \brief  Remove the blanks arround a string.
    ///
    /// \param str Source string.
    /// \return Cleaned string.
    static std::string_view removeSurroundingBlanks(const std::string& str);

    /// \brief  Remove the blanks arround a string.
    ///
    /// \param  str Source string.
    /// \return Cleaned string.
    static std::string_view& removeSurroundingBlanks(std::string_view& str);

    /// \brief  Split a string by white spaces and a list of delimiters.
    ///
    /// \param str Source string.
    /// \param delimiters List of delimiters.
    /// \return std::vector of substrings.
    static std::vector<std::string_view>
    splitString(const std::string& str, const std::initializer_list<const char> delimiters = {});

    /// \brief  Split a string by white spaces and a list of delimiters.
    ///
    /// \param str Source string.
    /// \param delimiters List of delimiters.
    /// \return std::vector of substrings.
    static std::vector<std::string_view>
    splitString(std::string_view str, const std::initializer_list<const char> delimiters = {});
};

} /* namespace ObjUtils */

#endif /* UTILS_H_ */
