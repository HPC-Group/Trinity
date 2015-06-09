/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


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

/**
  \file    RAWConverter.h
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \version 1.0
  \date    December 2008
*/


#pragma once

#ifndef RAWCONVERTER_H
#define RAWCONVERTER_H

#include <Core/StdTuvokDefines.h>
#include <list>
#include "AbstrConverter.h"

#include "../IOManager.h"  // for the size defines

typedef std::vector<std::pair<std::string, std::string>> KVPairs;

template<class T> class MinMaxScanner {
public:
  MinMaxScanner(LargeRAWFile* file, T& minValue, T& maxValue, uint64_t iElemCount) {
    size_t iMaxElemCount = size_t(std::min<uint64_t>(BLOCK_COPY_SIZE, iElemCount) /
                           sizeof(T));
    T* pInData = new T[iMaxElemCount];

    uint64_t iPos = 0;
    while (iPos < iElemCount)  {
      size_t iRead = file->ReadRAW((unsigned char*)pInData, iMaxElemCount*sizeof(T))/sizeof(T);
      if (iRead == 0) break;

      for (size_t i = 0;i<iRead;i++) {
        if (minValue > pInData[i]) minValue = pInData[i];
        if (maxValue < pInData[i]) maxValue = pInData[i];
      }
      iPos += uint64_t(iRead);
    }

    delete [] pInData;
  }
};

class RAWConverter : public AbstrConverter {
public:
  virtual ~RAWConverter() {}

  static bool ConvertRAWDataset(const std::string& strFilename,
                                const std::string& strTargetFilename,
                                const std::string& strTempDir,
                                uint64_t iHeaderSkip, unsigned iComponentSize,
                                uint64_t iComponentCount,
                                uint64_t timesteps,
                                bool bConvertEndianness, bool bSigned,
                                bool bIsFloat, Vec3ui64 vVolumeSize,
                                Vec3f vVolumeAspect,
                                const std::string& strDesc,
                                const std::string& strSource,
                                const uint64_t iTargetBrickSize,
                                const uint64_t iTargetBrickOverlap,
                                const bool bUseMedian,
                                const bool bClampToEdge,
                                uint32_t iBrickCompression,
                                uint32_t iBrickCompressionLevel,
                                uint32_t iBrickLayout,
                                KVPairs* pKVPairs = NULL,
                                const bool bQuantizeTo8Bit=false);

  static bool ExtractGZIPDataset(const std::string& strFilename,
                                 const std::string& strUncompressedFile,
                                 uint64_t iHeaderSkip);

  static bool ExtractBZIP2Dataset(const std::string& strFilename,
                                  const std::string& strUncompressedFile,
                                  uint64_t iHeaderSkip);

  static bool ParseTXTDataset(const std::string& strFilename,
                              const std::string& strBinaryFile,
                              uint64_t iHeaderSkip, unsigned iComponentSize,
                              uint64_t iComponentCount, bool bSigned,
                              bool bIsFloat, Vec3ui64 vVolumeSize);

  static bool AppendRAW(const std::string& strRawFilename, uint64_t iHeaderSkip,
                        const std::string& strTargetFilename,
                        unsigned iComponentSize,
                        bool bChangeEndianess=false,
                        bool bToSigned=false,
                        const bool bQuantizeTo8Bit=false);

  virtual bool ConvertToNative(const std::string& strRawFilename,
                               const std::string& strTargetFilename,
                               uint64_t iHeaderSkip, unsigned iComponentSize,
                               uint64_t iComponentCount, bool bSigned,
                               bool bFloatingPoint, Vec3ui64 vVolumeSize,
                               Vec3f vVolumeAspect,
                               bool bNoUserInteraction,
                               const bool bQuantizeTo8Bit);

  virtual bool ConvertToUVF(const std::string& strSourceFilename,
                            const std::string& strTargetFilename,
                            const std::string& strTempDir,
                            const bool bNoUserInteraction,
                            const uint64_t iTargetBrickSize,
                            const uint64_t iTargetBrickOverlap,
                            const bool bUseMedian,
                            const bool bClampToEdge,
                            uint32_t iBrickCompression,
                            uint32_t iBrickCompressionLevel,
                            uint32_t iBrickLayout,
                            const bool bQuantizeTo8Bit);

  virtual bool ConvertToUVF(const std::list<std::string>& files,
                            const std::string& strTargetFilename,
                            const std::string& strTempDir,
                            const bool bNoUserInteraction,
                            const uint64_t iTargetBrickSize,
                            const uint64_t iTargetBrickOverlap,
                            const bool bUseMedian,
                            const bool bClampToEdge,
                            uint32_t iBrickCompression,
                            uint32_t iBrickCompressionLevel,
                            uint32_t iBrickLayout,
                            const bool bQuantizeTo8Bit);

  virtual bool Analyze(const std::string& strSourceFilename,
                       const std::string& strTempDir,
                       bool bNoUserInteraction, RangeInfo& info);

  static bool Analyze(const std::string& strSourceFilename, uint64_t iHeaderSkip,
                      unsigned iComponentSize, uint64_t iComponentCount,
                      bool bSigned, bool bFloatingPoint,
                      Vec3ui64 vVolumeSize,
                      RangeInfo& info);

  virtual bool CanExportData() const { return true; }
  virtual bool CanImportData() const { return true; }

  /// Removes the given file or directory.  Warns if the file could not be
  /// deleted.
  /// @return true if the remove succeeded.
  static bool Remove(const std::string &, AbstrDebugOut &);
};

#endif // RAWCONVERTER_H
