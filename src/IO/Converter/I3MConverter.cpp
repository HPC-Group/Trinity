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
  \file    I3MConverter.cpp
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \date    December 2008
*/

#include <fstream>
#include "I3MConverter.h"

#include <Core/IO/KeyValueFileParser.h>
#include "../TuvokIOError.h"
#include <Core/IO/FileTools.h>
using namespace Core::IO::FileTools;

using namespace std;

// 42 and 69, guess that is "best of both worlds" :-)
#define I3M_MAGIC 69426942
#define I3M_VERSION 1
#define MAX_I3M_VOLSIZE 128

using Core::Math::VECTOR4;

I3MConverter::I3MConverter()
{
  m_vConverterDesc = "ImageVis3D Mobile Data";
  m_vSupportedExt.push_back("I3M");
}

bool I3MConverter::ConvertToRAW(const std::string& strSourceFilename,
                                const std::string& strTempDir, bool,
                                uint64_t& iHeaderSkip, unsigned& iComponentSize,
                                uint64_t& iComponentCount,
                                bool& bConvertEndianess, bool& bSigned,
                                bool& bIsFloat, Vec3ui64& vVolumeSize,
                                Vec3f& vVolumeAspect,
                                std::string& strTitle,
                                std::string& strIntermediateFile,
                                bool& bDeleteIntermediateFile)
{
 IVDA_MESSAGEV("Attempting to convert an ImageVis3D mobile dataset %s", strSourceFilename.c_str());

  bDeleteIntermediateFile = true;
  strTitle    = "ImageVis3D Mobile data";
  iHeaderSkip = 0;

  // I3M files are always four component 8bit little endian
  // unsigned, whereas the first 3 component of the vector
  // are the normalized gradient/normal and the fourth is the
  // actual data value, so all we need to todo here is parse
  // the binary header for the size and aspect ratio and then
  // create an intermediate RAW file taking every fourth byte
  // after the header while doing so we make sure to write the
  // file in the endianess of this machine
  iComponentSize    = 8;
  iComponentCount   = 1;
  bIsFloat          = false;
  bSigned           = false;
  bConvertEndianess = false;

  LargeRAWFile I3MFile(strSourceFilename, 0);
  I3MFile.Open(false);

  if (!I3MFile.IsOpen()) {
    IVDA_ERRORV("Unable to open source file %s", strSourceFilename.c_str());
    return false;
  }

  // get file size -> used for verification later
  uint64_t ulFileLength = I3MFile.GetCurrentSize();

  // get magic -> should be I3M_MAGIC
  uint32_t iMagic;
  I3MFile.ReadData(iMagic, false);
  if (iMagic != I3M_MAGIC) {
    I3MFile.Close();
    IVDA_ERRORV("This is not a valid I3M file %s", strSourceFilename.c_str());
    return false;
  }
 IVDA_MESSAGE("I3M Magic OK");

  // get version number -> must match I3M_VERSION
  uint32_t iVersion;
  I3MFile.ReadData(iVersion, false);
  if (iVersion != I3M_VERSION) {
    I3MFile.Close();
    IVDA_ERRORV("Unsuported I3M version in file %s", strSourceFilename.c_str());
    return false;
  }
 IVDA_MESSAGE("I3M Version OK");

  // get volume size -> every dimension must be MAX_I3M_VOLSIZE or less
  uint32_t iSize;
  I3MFile.ReadData(iSize, false); vVolumeSize.x = iSize;
  I3MFile.ReadData(iSize, false); vVolumeSize.y = iSize;
  I3MFile.ReadData(iSize, false); vVolumeSize.z = iSize;
  if (vVolumeSize.x > MAX_I3M_VOLSIZE ||
      vVolumeSize.y > MAX_I3M_VOLSIZE ||
      vVolumeSize.z > MAX_I3M_VOLSIZE) {
    I3MFile.Close();
    IVDA_ERRORV("Invalid volume size detected in I3M file %s", strSourceFilename.c_str());
    return false;
  }
 IVDA_MESSAGEV("Volume Size (%llu x %llu x %llu) in I3M file OK",
          vVolumeSize.x, vVolumeSize.y, vVolumeSize.z);

  // at this point we can check if the file has the correct size
  if (  8*4                     /* eight 32bit fields in the header */
      + 4*vVolumeSize.volume()  /* four component 8bit volume */
      != ulFileLength) {
    I3MFile.Close();
    IVDA_ERRORV("The size of the I3M file %s "
            "does not match the information in its header.",
            strSourceFilename.c_str());
    return false;
  }
 IVDA_MESSAGEV("File Size (%llu) of I3M file OK", ulFileLength);

  // get volume aspect
  I3MFile.ReadData(vVolumeAspect.x, false);
  I3MFile.ReadData(vVolumeAspect.y, false);
  I3MFile.ReadData(vVolumeAspect.z, false);
 IVDA_MESSAGEV("Aspect Ratio (%g x %g x %g)",
          vVolumeAspect.x, vVolumeAspect.y, vVolumeAspect.z);

  if (vVolumeAspect.x <= 0) {
   IVDA_WARNING("aspect ratio in x-direction is zero or less, setting it to 1");
    vVolumeAspect.x = 1;
  }
  if (vVolumeAspect.y <= 0) {
   IVDA_WARNING("aspect ratio in y-direction is zero or less, setting it to 1");
    vVolumeAspect.y = 1;
  }
  if (vVolumeAspect.z <= 0) {
   IVDA_WARNING("aspect ratio in z-direction is zero or less, setting it to 1");
    vVolumeAspect.z = 1;
  }


  // header is completed all test passed, now we can read the volume,
  // simply copy every fourth byte to the target file

 IVDA_MESSAGE("I3M File header scan completed, converting volume...");

  strIntermediateFile = strTempDir + GetFilename(strSourceFilename) +
                        ".temp";

  LargeRAWFile RAWFile(strIntermediateFile, 0);
  RAWFile.Create();

  if (!RAWFile.IsOpen()) {
    IVDA_ERRORV("Unable to open intermediate file %s",
            strIntermediateFile.c_str());
    I3MFile.Close();
    return false;
  }


  unsigned char* pData = new unsigned char[4*size_t(vVolumeSize.volume())];
  // read the 4D vectors
  I3MFile.ReadRAW(pData, 4*vVolumeSize.volume());
  I3MFile.Close();
  // compress in-place
  for (uint32_t i = 1;i<vVolumeSize.volume();i++) pData[i] = pData[3+i*4];
  // write to target file
  RAWFile.WriteRAW(pData, vVolumeSize.volume());
  delete [] pData;
  RAWFile.Close();

 IVDA_MESSAGEV("Intermediate RAW file %s from I3M file %s created.",
          strIntermediateFile.c_str(), strSourceFilename.c_str());

  return true;
}

void I3MConverter::Compute8BitGradientVolumeInCore(unsigned char* pSourceData, unsigned char* pTargetData, const Vec3ui64& vVolumeSize) {
  for (size_t z = 0;z<size_t(vVolumeSize[2]);z++) {
   IVDA_MESSAGEV("Computing gradients in slice %u of %llu\n(%g%% completed)",
            static_cast<unsigned>(z+1), vVolumeSize[2],
            100.0f*float(z+1)/float(vVolumeSize[2]));
    for (size_t y = 0;y<size_t(vVolumeSize[1]);y++) {
      for (size_t x = 0;x<size_t(vVolumeSize[0]);x++) {

        // compute 3D positions
        size_t iCenter = x+size_t(vVolumeSize[0])*y+size_t(vVolumeSize[0])*size_t(vVolumeSize[1])*z;
        size_t iLeft   = iCenter;
        size_t iRight  = iCenter;
        size_t iTop    = iCenter;
        size_t iBottom = iCenter;
        size_t iFront  = iCenter;
        size_t iBack   = iCenter;

        Vec3f vScale(0,0,0);

        // handle borders
        if (x > 0)          {iLeft   = iCenter-1; vScale.x++;}
        if (x < vVolumeSize[0]-1) {iRight  = iCenter+1; vScale.x++;}
        if (y > 0)          {iTop    = iCenter-size_t(vVolumeSize[0]);vScale.y++;}
        if (y < vVolumeSize[1]-1) {iBottom = iCenter+size_t(vVolumeSize[0]);vScale.y++;}
        if (z > 0)          {iFront  = iCenter-size_t(vVolumeSize[0])*size_t(vVolumeSize[1]);vScale.z++;}
        if (z < vVolumeSize[2]-1) {iBack   = iCenter+size_t(vVolumeSize[0])*size_t(vVolumeSize[1]);vScale.z++;}

        // compte central differences
        Vec3f vGradient((float(pSourceData[iLeft]) -float(pSourceData[iRight]) )/(255*vScale.x),
                               (float(pSourceData[iTop])  -float(pSourceData[iBottom]))/(255*vScale.y),
                               (float(pSourceData[iFront])-float(pSourceData[iBack])  )/(255*vScale.z));
        // safe normalize
        vGradient.normalize(0);

        // quantize to 8bit
        VECTOR4<unsigned char>  vUCharGradient = VECTOR3<unsigned char>((vGradient * 127)+127);

        // store in expanded format
        pTargetData[0+iCenter*4] = vUCharGradient.x;
        pTargetData[1+iCenter*4] = vUCharGradient.y;
        pTargetData[2+iCenter*4] = vUCharGradient.z;
        pTargetData[3+iCenter*4] = pSourceData[iCenter];
      }
    }
  }
}

void I3MConverter::DownSample(LargeRAWFile& SourceRAWFile, unsigned char* pDenseData, const Vec3ui64& vVolumeSize, const Vec3ui64& vDSFactor) {
  Vec3ui64 vSmallSize = vVolumeSize/vDSFactor;

  uint64_t iNumDownsampledValues = vDSFactor.volume();
  size_t iTargetIndex = 0;
  size_t iSourceIndex = 0;
  for (size_t z = 0;z<size_t(vSmallSize[2]);z++) {
   IVDA_MESSAGEV("Downsampling data in slice %u of %llu\n(%g%% completed)",
            static_cast<unsigned>(z+1), vSmallSize[2],
            100.0f*float(z+1)/float(vSmallSize[2]));
    for (size_t y = 0;y<size_t(vSmallSize[1]);y++) {
      for (size_t x = 0;x<size_t(vSmallSize[0]);x++) {

        double fAccValue = 0;
        unsigned char cValue = 0;

        for (size_t w = 0;w<size_t(vDSFactor[2]);w++) {
          for (size_t v = 0;v<size_t(vDSFactor[1]);v++) {
            for (size_t u = 0;u<size_t(vDSFactor[0]);u++) {
                iSourceIndex = size_t((x*vDSFactor[0]+u) + (y*vDSFactor[1]+v) * vVolumeSize[0] + (z*vDSFactor[2]+w) * vVolumeSize[0] * vVolumeSize[1]);
                SourceRAWFile.Read<unsigned char>(&cValue, 1, iSourceIndex, 0);
                fAccValue += cValue;
            }
          }
        }

        fAccValue /= iNumDownsampledValues;

        pDenseData[iTargetIndex++] = (unsigned char)(fAccValue);
      }
    }
  }
}

bool I3MConverter::ConvertToNative(const std::string& strRawFilename,
                                   const std::string& strTargetFilename, uint64_t iHeaderSkip,
                                   unsigned iComponentSize, uint64_t iComponentCount, bool bSigned,
                                   bool bFloatingPoint, Vec3ui64 vVolumeSize,
                                   Vec3f vVolumeAspect, bool ,
                                   const bool ) {

  // some fitness checks first
  if (iComponentCount!=1) {
    IVDA_ERROR("I3M only supports scalar data");
    return false;
  }

  // next check the quantization and endianess of the volume
  // if it is not 8bit unsigned char -> convert it

  bool bDelete8BitFile = false;
  string str8BitFilename = strRawFilename;

  if (iComponentSize!=8 || bSigned) {
    LargeRAWFile rf(strRawFilename, iHeaderSkip);
    rf.Open(false);

    if(!rf.IsOpen()) {
      using namespace Tuvok::io;
      throw DSOpenFailed(strRawFilename.c_str(), "Could not quantize input.",
                         __FILE__, __LINE__);
    }

    const std::string quantizedFilname = strTargetFilename+".tmp";
    if (QuantizeTo8Bit(rf,quantizedFilname, iComponentSize,
                       vVolumeSize.volume(), bSigned,bFloatingPoint)) {
      iHeaderSkip = 0;
      str8BitFilename = quantizedFilname;
      bDelete8BitFile = true;
      iComponentSize = 8;
      bSigned = false;
      bFloatingPoint = false;
    }

    rf.Close();
  }


  // next check is size of the volume, if a dimension is bigger than
  // MAX_I3M_VOLSIZE -> downsample the volume, otherwise simply copy

  LargeRAWFile UCharDataFile(str8BitFilename, iHeaderSkip);
  UCharDataFile.Open(false);

  if (!UCharDataFile.IsOpen()) {
    if (bDelete8BitFile)
      IVDA_ERRORV("Unable to open temp file for reading %s", str8BitFilename.c_str());
    else
      IVDA_ERRORV("Unable to open input file for reading %s", str8BitFilename.c_str());
    return false;
  }

  Vec3f vfDownSampleFactor = Vec3f(vVolumeSize)/float(MAX_I3M_VOLSIZE);

  unsigned char* pDenseData = NULL;
  Vec3ui64 vI3MVolumeSize;
  if (vfDownSampleFactor.x <= 1 && vfDownSampleFactor.y <= 1 && vfDownSampleFactor.z <= 1) {
    // volume is small enougth -> simply read the data into the array
    vI3MVolumeSize = vVolumeSize;
    pDenseData = new unsigned char[size_t(vI3MVolumeSize.volume())];
    UCharDataFile.ReadRAW(pDenseData, vI3MVolumeSize.volume());
  } else {
    // volume has to be downsampled
    Vec3ui64 viDownSampleFactor(uint64_t(ceil(vfDownSampleFactor.x)),
                                   uint64_t(ceil(vfDownSampleFactor.y)),
                                   uint64_t(ceil(vfDownSampleFactor.z)));
    vI3MVolumeSize = vVolumeSize/viDownSampleFactor;
    pDenseData = new unsigned char[size_t(vI3MVolumeSize.volume())];

    DownSample(UCharDataFile, pDenseData, vVolumeSize, viDownSampleFactor);

    // adjust aspect ratio
    vVolumeAspect *= Vec3f(vVolumeSize) / Vec3f(vI3MVolumeSize);
  }
  UCharDataFile.Close();
  if (bDelete8BitFile) UCharDataFile.Delete();

  // compute the gradients and expand data to vector format
  unsigned char* pData = new unsigned char[size_t(4*vI3MVolumeSize.volume())];
  Compute8BitGradientVolumeInCore(pDenseData, pData, vI3MVolumeSize);
  delete [] pDenseData;

  // write data to file
  LargeRAWFile TargetI3MFile(strTargetFilename, 0);
  TargetI3MFile.Create();

  if (!TargetI3MFile.IsOpen()) {
    IVDA_ERRORV("Unable to open I3M file %s", strTargetFilename.c_str());
    delete [] pData;
    return false;
  }

 IVDA_MESSAGE("Writing header information to disk");

  // magic
  TargetI3MFile.WriteData<uint32_t>(I3M_MAGIC, false);
  // version
  TargetI3MFile.WriteData<uint32_t>(I3M_VERSION, false);
  // (subsampled) domain size
  TargetI3MFile.WriteData<uint32_t>(uint32_t(vI3MVolumeSize.x), false);
  TargetI3MFile.WriteData<uint32_t>(uint32_t(vI3MVolumeSize.y), false);
  TargetI3MFile.WriteData<uint32_t>(uint32_t(vI3MVolumeSize.z), false);
  // aspect ratio
  TargetI3MFile.WriteData(vVolumeAspect.x, false);
  TargetI3MFile.WriteData(vVolumeAspect.y, false);
  TargetI3MFile.WriteData(vVolumeAspect.z, false);

 IVDA_MESSAGE("Writing volume to disk");

  TargetI3MFile.WriteRAW(pData, 4*vI3MVolumeSize.volume());

  TargetI3MFile.Close();
  delete [] pData;

  return true;
}
