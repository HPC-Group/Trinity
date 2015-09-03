#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <sstream>
#include <string>
#include <vector>

#include <core/SilverBulletBase.h>

namespace Core {
  namespace StringTools {
    template <typename T>
    std::string ToString(const T& aValue)
    {
      std::stringstream ss;
      ss << aValue;
      return ss.str();
    }

    template <> inline std::string ToString<std::string>(const std::string& s)
    {
      return s;
    }

    template <> inline std::string ToString<std::wstring>(const std::wstring& s)
    {
      return std::string(s.begin(), s.end());
    }

    template <typename T>
    std::wstring ToWString(const T& aValue)
    {
      std::wstringstream ss;
      ss << aValue;
      return ss.str();
    }

    template <> inline std::wstring ToWString<std::wstring>(const std::wstring& s)
    {
      return s;
    }

    template <> inline std::wstring ToWString<std::string>(const std::string& s)
    {
      return std::wstring(s.begin(), s.end());
    }

    template <typename T> bool FromString(T& t, const std::string& s,
                                          std::ios_base& (*f)(std::ios_base&) = std::dec)
    {
      std::istringstream iss(s);
      return !(iss >> f >> t).fail();
    }


    template <typename T> bool FromString(T& t, const std::wstring& s,
                                          std::ios_base& (*f)(std::ios_base&) = std::dec)
    {
      std::wistringstream iss(s);
      return !(iss >> f >> t).fail();
    }

    template <typename T> T FromString(const std::string& s,
                                       std::ios_base& (*f)(std::ios_base&) = std::dec)
    {
      T t;
      std::istringstream iss(s);
      iss >> f >> t;
      return t;
    }

    template <typename T> T FromString(const std::wstring& s,
                                       std::ios_base& (*f)(std::ios_base&) = std::dec)
    {
      T t;
      std::wistringstream iss(s);
      iss >> f >> t;
      return t;
    }

    std::wstring ToLowerCase(const std::wstring& str);
    std::string ToLowerCase(const std::string& str);
    std::wstring ToUpperCase(const std::wstring& str);
    std::string ToUpperCase(const std::string& str);

    enum EProtectMode {
      PM_NONE = 0,
      PM_QUOTES,
      PM_BRACKETS,
      PM_CUSTOM_DELIMITER
    };
    std::vector<std::string> Tokenize(const std::string& strInput,
                                      EProtectMode mode = PM_QUOTES,
                                      char customOrOpeningDelimiter='{',
                                      char closingDelimiter='}');
    std::vector<std::wstring> Tokenize(const std::wstring& strInput,
                                       EProtectMode mode = PM_QUOTES,
                                       wchar_t customOrOpeningDelimiter=L'{',
                                       wchar_t closingDelimiter=L'{');

    std::wstring TrimStrLeft(const std::wstring &str,
                             const std::wstring& c = L" \r\n\t");
    std::string TrimStrLeft(const std::string &str,
                            const std::string& c = " \r\n\t");
    std::wstring TrimStrRight(const std::wstring &str,
                              const std::wstring& c = L" \r\n\t");
    std::string TrimStrRight(const std::string &str,
                             const std::string& c = " \r\n\t");
    std::wstring TrimStr(const std::wstring &str,
                         const std::wstring& c = L" \r\n\t");
    std::string TrimStr(const std::string &str,
                        const std::string& c = " \r\n\t");

    void ReplaceAll(std::string& input, const std::string& search,
                    const std::string& replace);
    void ReplaceAll(std::wstring& input, const std::wstring& search,
                    const std::wstring& replace);

    std::vector<std::string> simpleSplit(const std::string &s, char delim);
  }
}

#endif // STRINGTOOLS_H

/*
 The MIT License

 Copyright (c) 2014 HPC Group, Univeristy Duisburg-Essen

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */
