#include <cassert>
#include <algorithm>  // transform

#include "StringTools.h"


using namespace std;

namespace Core {
  
  namespace StringTools {
    vector<string> Tokenize(const string& strInput,
                            EProtectMode mode,
                            char const customOrOpeningDelimiter,
                            char const closingDelimiter)
    {
      vector<string> strElements;
      const wstring wstrInput(strInput.begin(), strInput.end());
      const string strCustomOrOpeningDelimter(1, customOrOpeningDelimiter);
      const wstring wstrCustomOrOpeningDelimiter(strCustomOrOpeningDelimter.begin(), strCustomOrOpeningDelimter.end());
      assert(wstrCustomOrOpeningDelimiter.size() == 1);
      const string strClosingDelimter(1, closingDelimiter);
      const wstring wstrClosingDelimiter(strClosingDelimter.begin(), strClosingDelimter.end());
      assert(wstrClosingDelimiter.size() == 1);
      
      vector<wstring> wstrElements = Tokenize(wstrInput, mode, wstrCustomOrOpeningDelimiter.at(0), wstrClosingDelimiter.at(0));
      for (size_t i=0; i<wstrElements.size(); ++i) {
        strElements.push_back(string(wstrElements[i].begin(), wstrElements[i].end()));
      }
      return strElements;
    }
    
    vector<wstring> Tokenize(const wstring& strInput,
                             EProtectMode mode,
                             wchar_t const customOrOpeningDelimiter,
                             wchar_t const closingDelimiter)
    {
      vector<wstring> strElements;
      switch (mode) {
        case PM_QUOTES :
        {
          wstring buf;
          wstringstream ss(strInput);
          bool bProtected = false;
          while (ss >> buf) {
            wstring cleanBuf = buf;
            if (cleanBuf[0] == L'\"')
              cleanBuf = cleanBuf.substr(1, cleanBuf.length()-1);
            
            if (!cleanBuf.empty() && cleanBuf[cleanBuf.size()-1] == L'\"')
              cleanBuf = cleanBuf.substr(0, cleanBuf.length()-1);
            
            if (bProtected) {
              size_t end = strElements.size()-1;
              strElements[end] = strElements[end] + L" " + cleanBuf;
            }
            else
              strElements.push_back(cleanBuf);
            
            if (buf[0] == L'\"')
              bProtected = true;
            if (buf[buf.size()-1] == L'\"')
              bProtected = false;
          }
        } break;
          
        case PM_BRACKETS :
        {
          int iLevel = 0;
          size_t iStart = 0;
          size_t i = 0;
          for (;i<strInput.size();i++) {
            if (strInput[i] == customOrOpeningDelimiter) {
              if (iLevel == 0)
                iStart++;
              iLevel++;
              continue;
            } else if (strInput[i] == closingDelimiter) {
              iLevel--;
              if (iLevel == 0) {
                if (i-iStart > 0)
                  strElements.push_back(strInput.substr(iStart, i-iStart));
                iStart = i+1;
              }
              continue;
            }
            switch (strInput[i]) {
              case L' ':
              case L'\n':
              case L'\r':
              case L'\t':
                if (iLevel == 0) {
                  if (i-iStart > 0)
                    strElements.push_back(strInput.substr(iStart, i-iStart));
                  iStart = i+1;
                }
                break;
            }
          }
          if (i-iStart > 0)
            strElements.push_back(strInput.substr(iStart, i-iStart));
        } break;
          
        case PM_CUSTOM_DELIMITER :
        {
          size_t iStart = 0;
          size_t i = 0;
          for (;i<strInput.size();i++) {
            if (strInput[i] == customOrOpeningDelimiter) {
              if (i-iStart > 0)
                strElements.push_back(strInput.substr(iStart, i-iStart));
              iStart = i+1;
            }
          }
          if (i-iStart > 0)
            strElements.push_back(strInput.substr(iStart, i-iStart));
        } break;
          
        default :
        {
          wstring buf;
          wstringstream ss(strInput);
          while (ss >> buf) strElements.push_back(buf);
        } break;
      }
      return strElements;
    }
    
    void ReplaceAll(string& input, const string& search, const string& replace) {
      size_t pos=0;
      while(pos<input.size())
      {
        pos=input.find(search,pos);
        if(pos==std::string::npos) break;
        input.replace(pos,search.size(),replace);
        pos +=replace.size();
      }
    }
    
    void ReplaceAll(wstring& input, const wstring& search, const wstring& replace) {
      size_t pos=0;
      while(pos<input.size())
      {
        pos=input.find(search,pos);
        if(pos==std::string::npos) break;
        input.replace(pos,search.size(),replace);
        pos +=replace.size();
      }
    }
    
    inline int myTolower(int c) {return tolower(static_cast<unsigned char>(c));}
    inline int myToupper(int c) {return toupper(static_cast<unsigned char>(c));}
    
    std::string TrimStrLeft( const std::string& Src, const std::string& c)
    {
      size_t p1 = Src.find_first_not_of(c);
      if (p1 == std::string::npos) {return std::string();}
      return Src.substr(p1);
    }
    
    std::string TrimStrRight( const std::string& Src, const std::string& c)
    {
      size_t p2 = Src.find_last_not_of(c);
      if (p2 == std::string::npos) {return std::string();}
      return Src.substr(0, p2+1);
    }
    
    std::string TrimStr( const std::string& Src, const std::string& c)
    {
      size_t p2 = Src.find_last_not_of(c);
      if (p2 == std::string::npos) {return std::string();}
      size_t p1 = Src.find_first_not_of(c);
      if (p1 == std::string::npos) p1 = 0;
      return Src.substr(p1, (p2-p1)+1);
    }
    
    std::wstring TrimStrLeft( const std::wstring& Src, const std::wstring& c)
    {
      size_t p1 = Src.find_first_not_of(c);
      if (p1 == std::wstring::npos) {return std::wstring();}
      return Src.substr(p1);
    }
    
    std::wstring TrimStrRight( const std::wstring& Src, const std::wstring& c)
    {
      size_t p2 = Src.find_last_not_of(c);
      if (p2 == std::wstring::npos) {return std::wstring();}
      size_t p1 = Src.find_first_not_of(c);
      if (p1 == std::wstring::npos) p1 = 0;
      return Src.substr(0, p2+1);
    }
    
    std::wstring TrimStr( const std::wstring& Src, const std::wstring& c)
    {
      size_t p2 = Src.find_last_not_of(c);
      if (p2 == std::wstring::npos) {return  std::wstring();}
      size_t p1 = Src.find_first_not_of(c);
      if (p1 == std::wstring::npos) p1 = 0;
      return Src.substr(p1, (p2-p1)+1);
    }
    
    wstring ToLowerCase(const wstring& str) {
      wstring result(str);
      transform(str.begin(), str.end(), result.begin(), myTolower);
      return result;
    }
    
    string ToLowerCase(const string& str) {
      string result(str);
      transform(str.begin(), str.end(), result.begin(), myTolower);
      return result;
    }
    
    wstring ToUpperCase(const wstring& str) {
      wstring result(str);
      transform(str.begin(), str.end(), result.begin(), myToupper);
      return result;
    }
    
    string ToUpperCase(const string& str) {
      string result(str);
      transform(str.begin(), str.end(), result.begin(), myToupper);
      return result;
    }

    std::vector<std::string> &simpleSplit(const std::string &s, char delim, std::vector<std::string> &elems) {
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
        elems.push_back(item);
      }
      return elems;
    }
    std::vector<std::string> simpleSplit(const std::string &s, char delim) {
      std::vector<std::string> elems;
      simpleSplit(s, delim, elems);
      return elems;
    }
  }
}

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