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

//!    File   : VGStudioConverter.cpp
//!    Author : Jens Krueger
//!             DFKI & MMCI, Saarbruecken
//!             SCI Institute, University of Utah
//!    Date   : June 2010
//
//!    Copyright (C) 2010 DFKI, MMCI, SCI Institute

#include <fstream>
#include "VGStudioConverter.h"
#include "../VGIHeaderParser.h"
#include <core/IO/KeyValueFileParser.h>
#include <core/IO/FileTools.h>
using namespace Core::IO::FileTools;
using Core::IO::KeyValueFileParser;
using Core::IO::KeyValPair;

using namespace std;


VGStudioConverter::VGStudioConverter()
{
  m_vConverterDesc = "VGStudio Data";
  m_vSupportedExt.push_back("VGI");
}

bool VGStudioConverter::ConvertToRAW(const std::string& strSourceFilename,
                            const std::string&, bool,
                            uint64_t& iHeaderSkip, unsigned& iComponentSize,
                            uint64_t& iComponentCount, bool& bConvertEndianess,
                            bool& bSigned, bool& bIsFloat,
                            Vec3ui64& vVolumeSize,
                            Vec3f& vVolumeAspect, std::string& strTitle,
                            std::string& strIntermediateFile,
                            bool& bDeleteIntermediateFile) {

 IVDA_MESSAGEV("Attempting to convert VGStudio dataset %s",
          strSourceFilename.c_str());

  bDeleteIntermediateFile = false;
  strTitle          = "VGStudio data";
  iHeaderSkip       = 0;
  bConvertEndianess = IsBigEndian();
  vVolumeAspect     = Vec3f(1,1,1);

  VGIHeaderParser parser(strSourceFilename);

  if (parser.FileReadable())  {
    KeyValPair* fileformat = parser.GetData("FILEFORMAT");
    if (fileformat == NULL) {
      IVDA_ERROR("Tag 'fileformat' not found!");
      return false;
    }
    if (fileformat->strValueUpper != "RAW"){
      IVDA_ERROR("Currenly, only raw VGStudio files are supported!");
      return false;
    }

    KeyValPair* filename = parser.GetData("NAME");
    if (filename == NULL) {
      IVDA_ERROR("Tag 'name' not found!");
      return false;
    }
    strIntermediateFile = GetPath(strSourceFilename) +
                          GetFilename(filename->strValue);

    KeyValPair* resolution = parser.GetData("SIZE");
    if (resolution == NULL || resolution->vuiValue.size() != 3) {
      IVDA_ERROR("Tag 'size' not found!");
      return false;
    }
    vVolumeSize = Vec3ui64(resolution->vuiValue);

    KeyValPair* bitsperelement = parser.GetData("BITSPERELEMENT");
    if (bitsperelement == NULL) {
      IVDA_ERROR("Tag 'bitsperelement' not found!");
      return false;
    }
    iComponentSize = bitsperelement->iValue;

    KeyValPair* datatype = parser.GetData("DATATYPE");

    if (datatype == NULL) {
      IVDA_ERROR("Tag 'datatype' not found!");
      return false;
    }
    if(datatype->strValueUpper == "UNSIGNED INTEGER") {
      bSigned = false;
      iComponentCount = 1;
      bIsFloat = false;
    } else if (datatype->strValueUpper == "FLOAT") {
      bSigned = true;
      iComponentCount = 1;
      bIsFloat = true;
    }
  } else return false;

  return true;
}
