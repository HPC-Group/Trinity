#ifndef FILETOOLS_H
#define FILETOOLS_H

#include "silverbullet/base/SilverBulletBase.h"
#include "silverbullet/base/StringTools.h"

#ifdef DETECTED_OS_WINDOWS
#define NOMINMAX
#include <windows.h>
#define LARGE_STAT_BUFFER struct __stat64
#else
#include <wchar.h>
#include <sys/stat.h>
typedef wchar_t WCHAR;
typedef unsigned char CHAR;
#define LARGE_STAT_BUFFER struct stat
#endif

namespace Core {
  namespace IO {
    namespace FileTools {
      
      bool hasAbsPath(const std::string& path);
      std::string abs2RelPath(const std::string& source,
                              const std::string& target);
      
      bool fileExists(const std::string& fileName);
      bool fileExists(const std::wstring& fileName);
      
      std::string getExt(const std::string& fileName);
      std::wstring getExt(const std::wstring& fileName);
      
      std::string getPath(const std::string& fileName);
      std::wstring getPath(const std::wstring& fileName);
      
      std::string getFilename(const std::string& fileName);
      std::wstring getFilename(const std::wstring& fileName);
            
      std::string canonicalizePath(const std::string& path);
      
      std::string findPath(const std::string& fileName, const std::string& path);
      std::wstring findPath(const std::wstring& fileName,
                            const std::wstring& path);
      
      std::string  removeExt(const std::string& fileName);
      std::wstring removeExt(const std::wstring& fileName);
      
      std::string  checkExt(const std::string& fileName,
                            const std::string& newext);
      std::wstring checkExt(const std::wstring& fileName,
                            const std::wstring& newext);
      
      std::string  changeExt(const std::string& fileName,
                             const std::string& newext);
      std::wstring changeExt(const std::wstring& fileName,
                             const std::wstring& newext);
      
      std::string  appendFilename(const std::string& fileName,
                                  const int iTag);
      std::wstring appendFilename(const std::wstring& fileName,
                                  const int iTag);
      std::string  appendFilename(const std::string& fileName,
                                  const std::string& tag);
      std::wstring appendFilename(const std::wstring& fileName,
                                  const std::wstring& tag);

      std::string assembleFullFilename(const std::string& dir,
                                       const std::string& name);
      std::wstring assembleFullFilename(const std::wstring& dir,
                                        const std::wstring& name);
      
      std::string  findNextSequenceName(const std::string& strFilename);
      std::wstring findNextSequenceName(const std::wstring& wStrFilename);
      
      std::string  findNextSequenceName(const std::string& fileName,
                                        const std::string& ext,
                                        const std::string& dir="");
      std::wstring findNextSequenceName(const std::wstring& fileName,
                                        const std::wstring& ext,
                                        const std::wstring& dir=L"");
      
      uint32_t findNextSequenceIndex(const std::string& fileName,
                                     const std::string& ext,
                                     const std::string& dir="");
      uint32_t findNextSequenceIndex(const std::wstring& fileName,
                                     const std::wstring& ext,
                                     const std::wstring& dir=L"");
      
      bool getHomeDirectory(std::string& path);
      bool getHomeDirectory(std::wstring& path);
      
      std::string findFileInDirs(const std::string& file,
                                 const std::vector<std::string> strDirs);

      
#ifdef DETECTED_OS_WINDOWS
      std::vector<std::wstring> getDirContents(const std::wstring& dir,
                                               const std::wstring& fileName=L"*",
                                               const std::wstring& ext=L"*");
      std::vector<std::string> getDirContents(const std::string& dir,
                                              const std::string& fileName="*",
                                              const std::string& ext="*");
#else
      std::vector<std::wstring> getDirContents(const std::wstring& dir,
                                               const std::wstring& fileName=L"*",
                                               const std::wstring& ext=L"");
      std::vector<std::string> getDirContents(const std::string& dir,
                                              const std::string& fileName="*",
                                              const std::string& ext="");
#endif
      
      std::vector<std::wstring> getSubDirList(const std::wstring& dir);
      std::vector<std::string> getSubDirList(const std::string& dir);
      
      bool getFileStats(const std::string& strFileName, LARGE_STAT_BUFFER& stat_buf);
      bool getFileStats(const std::wstring& wstrFileName, LARGE_STAT_BUFFER& stat_buf);
      
      bool isDirectory(const std::string& name);
      bool isDirectory(const std::wstring& name);
    }
  }
}

#endif // FILETOOLS_H

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

