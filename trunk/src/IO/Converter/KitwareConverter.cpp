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
  \file    KitwareConverter.cpp
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \date    December 2008
*/

#include <fstream>
#include "KitwareConverter.h"
#include <Core/IO/KeyValueFileParser.h>
#include <Core/IO/FileTools.h>
using namespace Core::IO::FileTools;
using Core::IO::KeyValueFileParser;
using Core::IO::KeyValPair;


using namespace std;


KitwareConverter::KitwareConverter()
{
  m_vConverterDesc = "Kitware MHD Data";
  m_vSupportedExt.push_back("MHD");
}

bool KitwareConverter::ConvertToRAW(
  const std::string& strSourceFilename, const std::string&, bool,
  uint64_t& iHeaderSkip, unsigned& iComponentSize, uint64_t& iComponentCount,
  bool& bConvertEndianess, bool& bSigned, bool& bIsFloat,
  Vec3ui64& vVolumeSize, Vec3f& vVolumeAspect,
  std::string& strTitle, std::string& strIntermediateFile,
  bool& bDeleteIntermediateFile
) {
 IVDA_MESSAGEV("Attempting to convert Kitware MHD dataset %s", strSourceFilename.c_str());

  strTitle          = "Kitware MHD data";

  KeyValueFileParser parser(strSourceFilename,false,"=");

  if (parser.FileReadable())  {
    KeyValPair* dims           = parser.GetData("NDIMS");
    KeyValPair* dimsize        = parser.GetData("DIMSIZE");
    KeyValPair* ElementSpacing = parser.GetData("ELEMENTSPACING");
    KeyValPair* BigEndianFlag  = parser.GetData("ELEMENTBYTEORDERMSB");
    if (BigEndianFlag == NULL) BigEndianFlag = parser.GetData("BINARYDATABYTEORDERMSB");
    KeyValPair* ElementType    = parser.GetData("ELEMENTTYPE");
    KeyValPair* CompressedData = parser.GetData("COMPRESSEDDATA");
    KeyValPair* BinaryData      = parser.GetData("BINARYDATA");
    KeyValPair* Position       = parser.GetData("POSITION");
    KeyValPair* ElementNumberOfChannels = parser.GetData("ELEMENTNUMBEROFCHANNELS");
    KeyValPair* ElementDataFile = parser.GetData("ELEMENTDATAFILE");
    KeyValPair* HeaderSize = parser.GetData("HEADERSIZE");
    KeyValPair* ObjectType = parser.GetData("OBJECTTYPE");

    if (ObjectType && ObjectType->strValueUpper != "IMAGE") {
      IVDA_ERROR("Only image type MHD file are currently supported.");
      return false;
    }

    if (ElementDataFile == NULL) {
      IVDA_ERRORV("Unable to find 'ElementDataFile' tag in file %s.", strSourceFilename.c_str());
      return false;
    }

    if (dimsize == NULL) {
      IVDA_ERRORV("Unable to find 'DimSize' tag in file %s.", strSourceFilename.c_str());
      return false;
    }

    if (ElementType == NULL) {
      IVDA_ERRORV("Unable to find 'ElementType' tag in file %s.", strSourceFilename.c_str());
      return false;
    }

    if (BigEndianFlag == NULL) {
     IVDA_MESSAGEV("Unable to find 'ElementByteOrderMSB' or 'BinaryDataByteOrderMSB' tags in file %s assuming little endian data.", strSourceFilename.c_str());
      bConvertEndianess = IsBigEndian();
    } else {
      if(BigEndianFlag->strValueUpper == "FALSE") {
        bConvertEndianess = IsBigEndian();
      } else {
        bConvertEndianess = IsLittleEndian();
      }
    }

    if(ElementType->strValueUpper == "MET_CHAR") {
      bSigned = true;
      iComponentSize = 8;
      bIsFloat = false;
    } else if (ElementType->strValueUpper == "MET_UCHAR") {
      bSigned = false;
      iComponentSize = 8;
      bIsFloat = false;
    } else if (ElementType->strValueUpper == "MET_SHORT") {
      bSigned = true;
      iComponentSize = 16;
      bIsFloat = false;
    }else if (ElementType->strValueUpper == "MET_USHORT") {
      bSigned = false;
      iComponentSize = 16;
      bIsFloat = false;
    } else if (ElementType->strValueUpper == "MET_INT") {
      bSigned = true;
      iComponentSize = 32;
      bIsFloat = false;
    } else if (ElementType->strValueUpper == "MET_UINT") {
      bSigned = false;
      iComponentSize = 32;
      bIsFloat = false;
    } else if (ElementType->strValueUpper == "MET_FLOAT") {
      bSigned = true;
      iComponentSize = 32;
      bIsFloat = true;
    } else if (ElementType->strValueUpper == "MET_DOUBLE") {
      bSigned = true;
      iComponentSize = 64;
      bIsFloat = true;
    }


    if (ElementNumberOfChannels == NULL) {
     IVDA_MESSAGEV("Unable to find 'ElementNumberOfChannels' tag in file '%s'; "
              "assuming scalar data.", strSourceFilename.c_str());
      iComponentCount = 1;
    } else {
      iComponentCount = ElementNumberOfChannels->iValue;
    }

    strIntermediateFile = ElementDataFile->strValue;

    if (strIntermediateFile == "LIST") {
      IVDA_ERROR("LISTS are currently not supported in MHD files.");
      return false;
    }

    uint32_t iDims = static_cast<uint32_t>(dimsize->vuiValue.size());

    if (dims == NULL) {
     IVDA_WARNINGV("Unable to find 'NDims' tag in file %s relying on 'DimSize' tag.", strSourceFilename.c_str());
    } else {
      if (iDims != dims->uiValue) {
        IVDA_ERRORV("Tags 'NDims' and 'DimSize' are incosistent in file %s.", strSourceFilename.c_str());
        return false;
      }
    }

    if (iDims > 3) {
      IVDA_ERROR("Currently only up to 3D data supported.");
      return false;
    }

    vVolumeSize = Vec3ui64(dimsize->vuiValue, 1);
    vVolumeAspect = Vec3f(ElementSpacing->vfValue,1.0f);

    if (Position != NULL) {
      for (size_t i = 0;i<ElementSpacing->vfValue.size();i++) {
        if (ElementSpacing->vfValue[i] != 0.0f) {
         IVDA_WARNING("Ignoring non zero position.");
          break;
        }
      }
    }

    // TODO: find a non binary MHD payload file and figure out its format
    if (BinaryData != NULL) {
      if(BinaryData->strValueUpper == "FALSE") {
        IVDA_ERROR("Currently only binary MHD data supported.");
        return false;
      }
    }

    // TODO: find a compressed MHD payload file and figure out its format
    if (CompressedData != NULL) {
      if(CompressedData->strValueUpper == "TRUE") {
        IVDA_ERROR("Currently only uncompressed MHD data supported.");
        return false;
      }
    }
    bDeleteIntermediateFile = false;

    strIntermediateFile = GetPath(strSourceFilename) + strIntermediateFile;

    if (HeaderSize != NULL) {
      if (dimsize->iValue != -1 ) { // size -1 means compute header size automatically
        iHeaderSkip = dimsize->uiValue;
      } else {
        LargeRAWFile f(strIntermediateFile);
        if (f.Open(false)) {
          uint64_t iFileSize = f.GetCurrentSize();
          f.Close();
          iHeaderSkip = iFileSize - (iComponentSize/8)*vVolumeSize.volume()*iComponentCount;
        } else {
          IVDA_ERRORV("Unable to open paload file %s.", strIntermediateFile.c_str());
          return false;
        }
      }
    } else {
      iHeaderSkip = 0;
    }

  } else return false;

  return true;
}

bool KitwareConverter::ConvertToNative(const std::string& strRawFilename, const std::string& strTargetFilename, uint64_t iHeaderSkip,
                             unsigned iComponentSize, uint64_t iComponentCount, bool bSigned, bool bFloatingPoint,
                             Vec3ui64 vVolumeSize,Vec3f vVolumeAspect, bool bNoUserInteraction,
                             const bool bQuantizeTo8Bit) {

  // compute fromat string
  string strFormat;
  if (!bQuantizeTo8Bit) {
    if (bFloatingPoint && bSigned && iComponentSize == 64)
      strFormat = "MET_DOUBLE";
    else
    if (bFloatingPoint && bSigned && iComponentSize == 32)
      strFormat = "MET_FLOAT";
    else
    if (!bFloatingPoint && bSigned && iComponentSize == 32)
      strFormat = "MET_INT";
    else
    if (!bFloatingPoint && !bSigned && iComponentSize == 32)
      strFormat = "MET_UINT";
    else
    if (!bFloatingPoint && bSigned && iComponentSize == 8)
      strFormat = "MET_CHAR";
    else
    if (!bFloatingPoint && !bSigned && iComponentSize == 8)
      strFormat = "MET_UCHAR";
    else
    if (!bFloatingPoint && bSigned && iComponentSize == 16)
      strFormat = "MET_SHORT";
    else
    if (!bFloatingPoint && !bSigned && iComponentSize == 16)
      strFormat = "MET_USHORT";
    else {
      IVDA_ERROR("This data type is not supported by the MHD writer.");
      return false;
    }
  } else {
    if (bSigned)
      strFormat = "MET_CHAR";
    else
      strFormat = "MET_UCHAR";
  }


  // create textfile from metadata
  string strTargetRAWFilename = strTargetFilename+".raw";

  ofstream fTarget(strTargetFilename.c_str());
  if (!fTarget.is_open()) {
    IVDA_ERRORV("Unable to open target file %s.", strTargetFilename.c_str());
    return false;
  }

 IVDA_MESSAGE("Writing MHD File");

  fTarget << "ObjectType              = Image" << endl;
  fTarget << "BinaryData              = True" << endl;
  if (IsBigEndian())
    fTarget << "BinaryDataByteOrderMSB  = true" << endl;
  else
    fTarget << "BinaryDataByteOrderMSB  = false" << endl;
  fTarget << "HeaderSize              = 0" << endl;

  fTarget << "NDims                   = 3" << endl;
  fTarget << "DimSize                 = " << vVolumeSize.x << " " << vVolumeSize.y << " "<< vVolumeSize.z << endl;
  fTarget << "ElementSpacing          = " << vVolumeAspect.x << " " << vVolumeAspect.y << " "<< vVolumeAspect.z << endl;

  fTarget << "ElementNumberOfChannels = " << iComponentCount << endl;
  fTarget << "ElementType             = " << strFormat << endl;
  fTarget << "ElementDataFile         = " << GetFilename(strTargetRAWFilename) << endl;
  fTarget.close();

 IVDA_MESSAGE("Writing RAW File");

  // copy RAW file using the parent's call
  bool bRAWSuccess = RAWConverter::ConvertToNative(strRawFilename, strTargetRAWFilename, iHeaderSkip,
                                                   iComponentSize, iComponentCount, bSigned, bFloatingPoint,
                                                   vVolumeSize, vVolumeAspect, bNoUserInteraction,bQuantizeTo8Bit);

  if (bRAWSuccess) {
    return true;
  } else {
    IVDA_ERRORV("Error creating raw target file %s.", strTargetRAWFilename.c_str());
    remove(strTargetFilename.c_str());
    return false;
  }
}
