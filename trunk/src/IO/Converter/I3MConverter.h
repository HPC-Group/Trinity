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
  \file    I3MConverter.h
  \author  Jens Krueger
           DFKI Saarbruecken and SCI Institute, University of Utah
  \version 1.0
  \date    July 2009
*/
#pragma once

#ifndef I3MCONVERTER_H
#define I3MCONVERTER_H

#include <Core/StdTuvokDefines.h>
#include "RAWConverter.h"

class I3MConverter : public RAWConverter {
public:
  I3MConverter();
  virtual ~I3MConverter() {}

  virtual bool ConvertToRAW(
    const std::string& strSourceFilename,
    const std::string& strTempDir, bool bNoUserInteraction,
    uint64_t& iHeaderSkip, unsigned& iComponentSize, uint64_t& iComponentCount,
    bool& bConvertEndianess, bool& bSigned, bool& bIsFloat,
    Vec3ui64& vVolumeSize, Vec3f& vVolumeAspect,
    std::string& strTitle, std::string& strIntermediateFile,
    bool& bDeleteIntermediateFile
  );

  virtual bool ConvertToNative(const std::string& strRawFilename, const std::string& strTargetFilename, uint64_t iHeaderSkip,
                               unsigned iComponentSize, uint64_t iComponentCount, bool bSigned, bool bFloatingPoint,
                               Vec3ui64 vVolumeSize,Vec3f vVolumeAspect, bool bNoUserInteraction,
                               const bool bQuantizeTo8Bit);

  virtual bool CanExportData() const {return true;}
  virtual bool CanImportData() const { return true; }

protected:
  static void Compute8BitGradientVolumeInCore(unsigned char* pSourceData, unsigned char* pTargetData, const Vec3ui64& vVolumeSize);
  static void DownSample(LargeRAWFile& SourceRAWFile, unsigned char* pDenseData, const Vec3ui64& vVolumeSize, const Vec3ui64& viDownSampleFactor);

};

#endif // I3MCONVERTER_H
