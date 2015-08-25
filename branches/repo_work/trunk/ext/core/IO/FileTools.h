#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <core/SilverBulletBase.h>

#include "../StringTools.h"

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

      std::string GetFromResourceOnMac(const std::string& strFileName);

      bool HasAbsPath(const std::string& path);
      std::string Abs2RelPath(const std::string& source,
                              const std::string& target);

      bool FileExists(const std::string& fileName);
      bool FileExists(const std::wstring& fileName);

      std::string GetExt(const std::string& fileName);
      std::wstring GetExt(const std::wstring& fileName);

      std::string GetPath(const std::string& fileName);
      std::wstring GetPath(const std::wstring& fileName);

      std::string GetFilename(const std::string& fileName);
      std::wstring GetFilename(const std::wstring& fileName);

      std::string CanonicalizePath(const std::string& path);

      std::string FindPath(const std::string& fileName, const std::string& path);
      std::wstring FindPath(const std::wstring& fileName,
                            const std::wstring& path);

      std::string  RemoveExt(const std::string& fileName);
      std::wstring RemoveExt(const std::wstring& fileName);

      std::string  CheckExt(const std::string& fileName,
                            const std::string& newext);
      std::wstring CheckExt(const std::wstring& fileName,
                            const std::wstring& newext);

      std::string  ChangeExt(const std::string& fileName,
                             const std::string& newext);
      std::wstring ChangeExt(const std::wstring& fileName,
                             const std::wstring& newext);

      std::string  AppendFilename(const std::string& fileName,
                                  const int iTag);
      std::wstring AppendFilename(const std::wstring& fileName,
                                  const int iTag);
      std::string  AppendFilename(const std::string& fileName,
                                  const std::string& tag);
      std::wstring AppendFilename(const std::wstring& fileName,
                                  const std::wstring& tag);

      std::string AssembleFullFilename(const std::string& dir,
                                       const std::string& name);
      std::wstring AssembleFullFilename(const std::wstring& dir,
                                        const std::wstring& name);

      std::string  FindNextSequenceName(const std::string& strFilename);
      std::wstring FindNextSequenceName(const std::wstring& wStrFilename);

      std::string  FindNextSequenceName(const std::string& fileName,
                                        const std::string& ext,
                                        const std::string& dir="");
      std::wstring FindNextSequenceName(const std::wstring& fileName,
                                        const std::wstring& ext,
                                        const std::wstring& dir=L"");

      uint32_t FindNextSequenceIndex(const std::string& fileName,
                                     const std::string& ext,
                                     const std::string& dir="");
      uint32_t FindNextSequenceIndex(const std::wstring& fileName,
                                     const std::wstring& ext,
                                     const std::wstring& dir=L"");

      bool GetHomeDirectory(std::string& path);
      bool GetHomeDirectory(std::wstring& path);

#ifdef DETECTED_OS_WINDOWS
      std::vector<std::wstring> GetDirContents(const std::wstring& dir,
                                               const std::wstring& fileName=L"*",
                                               const std::wstring& ext=L"*");
      std::vector<std::string> GetDirContents(const std::string& dir,
                                              const std::string& fileName="*",
                                              const std::string& ext="*");
#else
      std::vector<std::wstring> GetDirContents(const std::wstring& dir,
                                               const std::wstring& fileName=L"*",
                                               const std::wstring& ext=L"");
      std::vector<std::string> GetDirContents(const std::string& dir,
                                              const std::string& fileName="*",
                                              const std::string& ext="");
#endif

      std::vector<std::wstring> GetSubDirList(const std::wstring& dir);
      std::vector<std::string> GetSubDirList(const std::string& dir);

      bool GetFileStats(const std::string& strFileName, LARGE_STAT_BUFFER& stat_buf);
      bool GetFileStats(const std::wstring& wstrFileName, LARGE_STAT_BUFFER& stat_buf);
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

