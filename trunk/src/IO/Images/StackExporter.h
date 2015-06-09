/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2011 Scientific Computing and Imaging Institute,
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
  \file    StackExporter.h
  \author    Jens Krueger
        SCI Institute
        University of Utah
*/
#pragma once

#ifndef STACKEXPORTER_H
#define STACKEXPORTER_H

#include <vector>
#include <string>

#include <Core/StdTuvokDefines.h>
#include "../TransferFunction1D.h"
using namespace Core::Math;

class StackExporter
{
public:

  static std::vector<std::pair<std::string,std::string>> GetSuportedImageFormats();

  static bool WriteStacks(const std::string& strRAWFilename,
                          const std::string& strTargetFilename,
                          const DataIO::TransferFunction1D* pTrans,
                          uint64_t iBitWidth,
                          uint64_t iComponentCount,
                          float fRescale,
                          Vec3ui64 vDomainSize,
                          bool bAllDirs);

  static bool WriteImage(unsigned char* pData,
                  const std::string& strTargetFilename,
                  const Vec2ui64& vSize,
                  uint64_t iComponentCount);
protected:
  template<typename T> static void ApplyTFInplace(T* pData,
                             Vec2ui64 vSize,
                             float fRescale,
                             const DataIO::TransferFunction1D* pTrans) {

     unsigned char* pCharInterpOfData = (unsigned char*)pData;

     for (size_t i = 0;i<vSize.area();++i) {
      size_t sourcePos = size_t(vSize.area()-(i+1));
      size_t targetPos = 4 * sourcePos;

      size_t lookupVal = std::min(size_t(pData[sourcePos]*fRescale),
                                  pTrans->GetSize()-1);
      Vec4f fvColor = pTrans->GetColor(lookupVal);

      pCharInterpOfData[targetPos+0] = (unsigned char)(fvColor.x*255);
      pCharInterpOfData[targetPos+1] = (unsigned char)(fvColor.y*255);
      pCharInterpOfData[targetPos+2] = (unsigned char)(fvColor.z*255);
      pCharInterpOfData[targetPos+3] = (unsigned char)(fvColor.w*255);
    }

  }

  static void PadInplace(unsigned char* pData,
                  Vec2ui64 vSize,
                  unsigned int iStepping,
                  unsigned int iPadcount,
                  unsigned char iValue);

  static bool WriteSlice(unsigned char* pData,
                  const DataIO::TransferFunction1D* pTrans,
                  uint64_t iBitWidth,
                  const std::string& strCurrentDiFilename,
                  const Vec2ui64& vSize,
                  float fRescale,
                  uint64_t iComponentCount);


};

#endif // STACKEXPORTER_H
