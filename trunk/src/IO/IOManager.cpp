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
  \file    IOManager.cpp
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \date    August 2008
*/

#include <Core/StdTuvokDefines.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <float.h>
#include <iterator>
#include <set>
#include <sstream>
#include <map>
#include <memory>
#include <jpeglib/jconfig.h>

#include "IOManager.h"

#include <Core/Other/MC.h>
#include <Core/SystemInfo.h>

#include <Core/Controller/BasicsController.h>
#include "DSFactory.h"
#include "DynamicBrickingDS.h"
#include "exception/UnmergeableDatasets.h"

#ifdef COMPILE_EXPRESSIONS
#include "expressions/parser.h"
#include "expressions/syntax.h"
#include "expressions/treenode.h"
#endif

#include "IO/DICOM/DICOMParser.h"
#include "IO/Images/ImageParser.h"
#include "IO/Images/StackExporter.h"
#include "Quantize.h"
#include "TuvokJPEG.h"
#include "TransferFunction1D.h"
#include "TuvokSizes.h"
#include "uvfDataset.h"
#include "UVF/UVF.h"
#include "UVF/GeometryDataBlock.h"
#include "UVF/Histogram1DDataBlock.h"
#include "UVF/Histogram2DDataBlock.h"

#include "Converter/AmiraConverter.h"
#include "Converter/AnalyzeConverter.h"
#include "Converter/BOVConverter.h"
#include "Converter/IASSConverter.h"
#include "Converter/I3MConverter.h"
#include "Converter/InveonConverter.h"
#include "Converter/KitwareConverter.h"
#include "Converter/MRCConverter.h"
#include "Converter/NRRDConverter.h"
#include "Converter/QVISConverter.h"
#include "Converter/REKConverter.h"
#include "Converter/StkConverter.h"
#include "Converter/TiffVolumeConverter.h"
#include "Converter/VFFConverter.h"
#include "Converter/VGStudioConverter.h"
#include "Converter/VTKConverter.h"

#include <Core/Other/Mesh.h>
#include "GeoConverter/AbstrGeoConverter.h"
#include "GeoConverter/GeomViewConverter.h"
#include "GeoConverter/LinesGeoConverter.h"
#include "GeoConverter/MobileGeoConverter.h"
#include "GeoConverter/MedAlyVisGeoConverter.h"
#include "GeoConverter/MedAlyVisFiberTractGeoConverter.h"
#include "GeoConverter/OBJGeoConverter.h"
#include "GeoConverter/PLYGeoConverter.h"
#include "GeoConverter/XML3DGeoConverter.h"
#include "GeoConverter/StLGeoConverter.h"

#include <Core/IO/FileTools.h>
#include <Core/StringTools.h>
using namespace Core::IO::FileTools;
using namespace Core::StringTools;

using namespace std;
//using namespace boost;
using namespace Tuvok;
using namespace Core::Math;

static void read_first_block(const string& filename,
                             vector<int8_t>& block)
{
  ifstream ifs(filename.c_str(), ifstream::in |
                                      ifstream::binary);
  block.resize(512);
  ifs.read(reinterpret_cast<char*>(&block[0]), 512);
  ifs.close();
}

// Figure out the converters that can convert the given file.
// Multiple formats might think they can do as much; we return all of them and
// let the higher level figure it out.
namespace {
  template<typename ForwIter>
  std::set<std::shared_ptr<AbstrConverter>>
  identify_converters(const std::string& filename,
                      ForwIter cbegin, ForwIter cend)
  {
    std::set<std::shared_ptr<AbstrConverter>> converters;

    vector<int8_t> bytes(512);
    read_first_block(filename, bytes);

    while(cbegin != cend) {
      IVDA_MESSAGEV("Attempting converter '%s'", (*cbegin)->GetDesc().c_str());
      if((*cbegin)->CanRead(filename, bytes)) {
        IVDA_MESSAGEV("Converter '%s' can read '%s'!",
                (*cbegin)->GetDesc().c_str(), filename.c_str());
        converters.insert(*cbegin);
      }
      ++cbegin;
    }
    return converters;
  }
}

IOManager::IOManager() :
  m_dsFactory(new io::DSFactory()),
  m_iMaxBrickSize(DEFAULT_BRICKSIZE),
  m_iBuilderBrickSize(DEFAULT_BUILDER_BRICKSIZE),
  m_iBrickOverlap(DEFAULT_BRICKOVERLAP),
  m_iIncoresize(m_iMaxBrickSize*m_iMaxBrickSize*m_iMaxBrickSize),
  m_bUseMedianFilter(false),
  m_bClampToEdge(false),
  m_iCompression(1), // default zlib compression
  m_iCompressionLevel(1), // default compression level best speed
  m_iLayout(0), // default scanline layout
  m_LoadDS(nullptr)
{
  m_vpGeoConverters.push_back(new GeomViewConverter());
  m_vpGeoConverters.push_back(new LinesGeoConverter());
  m_vpGeoConverters.push_back(new MobileGeoConverter());
  m_vpGeoConverters.push_back(new MedAlyVisGeoConverter());
  m_vpGeoConverters.push_back(new MedAlyVisFiberTractGeoConverter());
  m_vpGeoConverters.push_back(new OBJGeoConverter());
  m_vpGeoConverters.push_back(new PLYGeoConverter());
  m_vpGeoConverters.push_back(new XML3DGeoConverter());
  m_vpGeoConverters.push_back(new StLGeoConverter());

  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new VGStudioConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new QVISConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new NRRDConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new StkConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new TiffVolumeConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new VFFConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new BOVConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new REKConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new IASSConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new I3MConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new KitwareConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new InveonConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new AnalyzeConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new AmiraConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new MRCConverter()));
  m_vpConverters.push_back(std::shared_ptr<AbstrConverter>(new VTKConverter()));
  m_dsFactory->AddReader(shared_ptr<UVFDataset>(new UVFDataset()));
}


void IOManager::RegisterExternalConverter(shared_ptr<AbstrConverter> pConverter) {
  m_vpConverters.push_back(pConverter);
}

void IOManager::RegisterFinalConverter(shared_ptr<AbstrConverter> pConverter) {
  m_pFinalConverter = pConverter;
}

namespace {
  template <typename T>
  void Delete(T *t) { delete t; }
}

IOManager::~IOManager()
{
  std::for_each(m_vpGeoConverters.begin(), m_vpGeoConverters.end(),
                Delete<AbstrGeoConverter>);
  m_vpConverters.clear();
  m_vpGeoConverters.clear();

  m_pFinalConverter.reset();
}

vector<std::shared_ptr<FileStackInfo>>
IOManager::ScanDirectory(string strDirectory) const {
  IVDA_MESSAGEV("Scanning directory %s", strDirectory.c_str());

  vector<std::shared_ptr<FileStackInfo>> fileStacks;

  DICOMParser parseDICOM;
  parseDICOM.GetDirInfo(strDirectory);

  // Sort out DICOMs with embedded images that we can't read.
  for (size_t stack=0; stack < parseDICOM.m_FileStacks.size(); ++stack) {
    std::auto_ptr<DICOMStackInfo> f = std::auto_ptr<DICOMStackInfo>(
      new DICOMStackInfo((DICOMStackInfo*)parseDICOM.m_FileStacks[stack])
    );

    // if trying to load JPEG files. check if we can handle the JPEG payload
    if (f->m_bIsJPEGEncoded) {
      for(size_t i=0; i < f->m_Elements.size(); ++i) {
        if(!Tuvok::JPEG(f->m_Elements[i]->m_strFileName,
                        dynamic_cast<SimpleDICOMFileInfo*>
                          (f->m_Elements[i])->GetOffsetToData()).valid()) {
         IVDA_WARNINGV("Can't load JPEG in stack %u, element %u!",
                  static_cast<unsigned>(stack), static_cast<unsigned>(i));
          // should probably be using ptr container lib here instead of
          // trying to explicitly manage this.
          delete *(parseDICOM.m_FileStacks.begin()+stack);
          parseDICOM.m_FileStacks.erase(parseDICOM.m_FileStacks.begin()+stack);
          stack--;
          break;
        }
      }
    }
  }

  if (parseDICOM.m_FileStacks.size() == 1) {
    IVDA_MESSAGE("  found a single DICOM stack");
  } else {
    IVDA_MESSAGEV("  found %u DICOM stacks",
            static_cast<unsigned>(parseDICOM.m_FileStacks.size()));
  }

  for (size_t stack=0; stack < parseDICOM.m_FileStacks.size(); ++stack) {
    std::shared_ptr<FileStackInfo> f =
      std::shared_ptr<FileStackInfo>(new DICOMStackInfo(
        static_cast<DICOMStackInfo*>(parseDICOM.m_FileStacks[stack])
      ));

    stringstream s;
    s << f->m_strFileType << " Stack: " << f->m_strDesc;
    f->m_strDesc = s.str();

    fileStacks.push_back(f);
  }

  ImageParser parseImages;
  parseImages.GetDirInfo(strDirectory);

  if (parseImages.m_FileStacks.size() == 1) {
    IVDA_MESSAGE("  found a single image stack");
  } else {
    IVDA_MESSAGEV("  found %u image stacks",
            static_cast<unsigned>(parseImages.m_FileStacks.size()));
  }

  for (size_t stack=0; stack < parseImages.m_FileStacks.size(); ++stack) {
    std::shared_ptr<FileStackInfo> f =
      std::shared_ptr<FileStackInfo>(new ImageStackInfo(
        dynamic_cast<ImageStackInfo*>(parseImages.m_FileStacks[stack])
      ));

    stringstream s;
    s << f->m_strFileType << " Stack: " << f->m_strDesc;
    f->m_strDesc = s.str();

    fileStacks.push_back(f);
  }

   // add other image parsers here

  IVDA_MESSAGE("  scan complete");

  return fileStacks;
}

#ifdef DETECTED_OS_WINDOWS
  #pragma warning(disable:4996)
#endif

bool IOManager::ConvertDataset(FileStackInfo* pStack,
                               const string& strTargetFilename,
                               const string& strTempDir,
                               const uint64_t iMaxBrickSize,
                               uint64_t iBrickOverlap,
                               const bool bQuantizeTo8Bit) const {
  IVDA_MESSAGEV("Request to convert stack of %s files to %s received",
          pStack->m_strDesc.c_str(), strTargetFilename.c_str());

  if (pStack->m_strFileType == "DICOM") {
    IVDA_MESSAGE("  Detected DICOM stack, starting DICOM conversion");

    DICOMStackInfo* pDICOMStack = ((DICOMStackInfo*)pStack);

    IVDA_MESSAGEV("  Stack contains %u files",
            static_cast<unsigned>(pDICOMStack->m_Elements.size()));
    IVDA_MESSAGEV("    Series: %u  Bits: %u (%u)", pDICOMStack->m_iSeries,
                                             pDICOMStack->m_iAllocated,
                                             pDICOMStack->m_iStored);
    IVDA_MESSAGEV("    Date: %s  Time: %s", pDICOMStack->m_strAcquDate.c_str(),
                                      pDICOMStack->m_strAcquTime.c_str());
    IVDA_MESSAGEV("    Modality: %s  Description: %s",
            pDICOMStack->m_strModality.c_str(),
            pDICOMStack->m_strDesc.c_str());
    IVDA_MESSAGEV("    Aspect Ratio: %g %g %g", pDICOMStack->m_fvfAspect.x,
            pDICOMStack->m_fvfAspect.y, pDICOMStack->m_fvfAspect.z);

    string strTempMergeFilename = strTempDir +
                                  GetFilename(strTargetFilename) +
                                  "~";
    IVDA_MESSAGEV("Creating intermediate file %s", strTempMergeFilename.c_str());

    ofstream fs;
    fs.open(strTempMergeFilename.c_str(), fstream::binary);
    if (fs.fail()) {
      IVDA_ERRORV("Could not create temp file %s aborted conversion.",
              strTempMergeFilename.c_str());
      return false;
    }

    vector<char> vData;
    for (size_t j=0; j < pDICOMStack->m_Elements.size(); j++) {

      SimpleDICOMFileInfo* pDICOMFileInfo = dynamic_cast<SimpleDICOMFileInfo*>(pDICOMStack->m_Elements[j]);

      if (!pDICOMFileInfo) continue;

      uint32_t iDataSize = pDICOMStack->m_Elements[j]->GetDataSize();
      vData.resize(iDataSize);

      if (pDICOMStack->m_bIsJPEGEncoded) {
        IVDA_MESSAGEV("JPEG is %d bytes, offset %d", iDataSize,
                pDICOMFileInfo->GetOffsetToData());
        Tuvok::JPEG jpg(pDICOMStack->m_Elements[j]->m_strFileName,
                        pDICOMFileInfo->GetOffsetToData());
        if(!jpg.valid()) {
          IVDA_ERRORV("'%s' reports an embedded JPEG, but the JPEG is invalid.",
                  pDICOMStack->m_Elements[j]->m_strFileName.c_str());
          return false;
        }
        IVDA_MESSAGEV("jpg is: %u bytes (%ux%u, %u components)", uint32_t(jpg.size()),
                uint32_t(jpg.width()), uint32_t(jpg.height()),
                uint32_t(jpg.components()));

        const char *jpeg_data = jpg.data();
        copy(jpeg_data, jpeg_data + jpg.size(), &vData[0]);
        pDICOMStack->m_iAllocated = BITS_IN_JSAMPLE;
      } else {
        pDICOMStack->m_Elements[j]->GetData(vData);
        IVDA_MESSAGEV("Creating intermediate file %s\n%u%%",
                strTempMergeFilename.c_str(),
                static_cast<unsigned>((100*j)/pDICOMStack->m_Elements.size()));
      }

      if (pDICOMStack->m_bIsBigEndian != EndianConvert::IsBigEndian()) {
        IVDA_MESSAGE("Converting Endianess ...");
        switch (pDICOMStack->m_iAllocated) {
          case  8 : break;
          case 16 : {
                short *pData = reinterpret_cast<short*>(&vData[0]);
                for (uint32_t k = 0;k<iDataSize/2;k++)
                  pData[k] = EndianConvert::Swap<short>(pData[k]);
                } break;
          case 32 : {
                int *pData = reinterpret_cast<int*>(&vData[0]);
                for (uint32_t k = 0;k<iDataSize/4;k++)
                  pData[k] = EndianConvert::Swap<int>(pData[k]);
                } break;
        }
      }

      // HACK: For now we set bias to 0 for unsigned file as we've
      // encountered a number of DICOM files files where the bias
      // parameter would create negative values and so far I don't know
      // how to interpret this correctly
      if (!pDICOMStack->m_bSigned) pDICOMFileInfo->m_fBias = 0.0f;

      if (pDICOMFileInfo->m_fScale != 1.0f || pDICOMFileInfo->m_fBias != 0.0f) {
        IVDA_MESSAGE("Applying Scale and Bias  ...");
        if (pDICOMStack->m_bSigned) {
          switch (pDICOMStack->m_iAllocated) {
            case  8 :{
                  char *pData = reinterpret_cast<char*>(&vData[0]);
                  for (uint32_t k = 0;k<iDataSize/2;k++){
                    float sbValue = pData[k] * pDICOMFileInfo->m_fScale + pDICOMFileInfo->m_fBias;
                    pData[k] = (char)(sbValue);
                  }} break;
            case 16 : {
                  short *pData = reinterpret_cast<short*>(&vData[0]);
                  for (uint32_t k = 0;k<iDataSize/2;k++){
                    float sbValue = pData[k] * pDICOMFileInfo->m_fScale + pDICOMFileInfo->m_fBias;
                    pData[k] = (short)(sbValue);
                  }} break;
            case 32 : {
                  int *pData = reinterpret_cast<int*>(&vData[0]);
                  for (uint32_t k = 0;k<iDataSize/4;k++){
                    float sbValue = pData[k] * pDICOMFileInfo->m_fScale + pDICOMFileInfo->m_fBias;
                    pData[k] = (int)(sbValue);
                  }} break;
          }
        } else {
          switch (pDICOMStack->m_iAllocated) {
            case  8 :{
                  unsigned char *pData = reinterpret_cast<unsigned char*>(&vData[0]);
                  for (uint32_t k = 0;k<iDataSize/2;k++){
                    float sbValue = pData[k] * pDICOMFileInfo->m_fScale + pDICOMFileInfo->m_fBias;
                    pData[k] = (unsigned char)(sbValue);
                  }} break;
            case 16 : {
                  unsigned short *pData = reinterpret_cast<unsigned short*>(&vData[0]);
                  for (uint32_t k = 0;k<iDataSize/2;k++){
                    float sbValue = pData[k] * pDICOMFileInfo->m_fScale + pDICOMFileInfo->m_fBias;
                    pData[k] = (unsigned short)(sbValue);
                  }} break;
            case 32 : {
                  unsigned int *pData = reinterpret_cast<unsigned int*>(&vData[0]);
                  for (uint32_t k = 0;k<iDataSize/4;k++) {
                    float sbValue = pData[k] * pDICOMFileInfo->m_fScale + pDICOMFileInfo->m_fBias;
                    pData[k] = (unsigned int)(sbValue);
                  }} break;
          }
        }
      }

      // TODO: implement proper DICOM Windowing
      if (pDICOMFileInfo->m_fWindowWidth > 0) {
       IVDA_WARNING("DICOM Windowing parameters found!");
      }

      // Create temporary file with the DICOM (image) data.  We pretend 3
      // component data is 4 component data to simplify processing later.
      /// @todo FIXME: this code assumes 3 component data is always 3*char
      if (pDICOMStack->m_iComponentCount == 3) {
        uint32_t iRGBADataSize = (iDataSize / 3) * 4;

        // Later we'll tell RAWConverter that this dataset has
        // m_iComponentCount components.  Since we're upping the number of
        // components here, we update the component count too.
        pDICOMStack->m_iComponentCount = 4;
        // Do note that the number of components in the data and the number of
        // components in our in-memory copy of the data now differ.

        unsigned char *pRGBAData = new unsigned char[iRGBADataSize];
        for (uint32_t k = 0;k<iDataSize/3;k++) {
          pRGBAData[k*4+0] = vData[k*3+0];
          pRGBAData[k*4+1] = vData[k*3+1];
          pRGBAData[k*4+2] = vData[k*3+2];
          pRGBAData[k*4+3] = 255;
        }
        fs.write((char*)pRGBAData, iRGBADataSize);
        delete[] pRGBAData;
      } else {
        fs.write(&vData[0], iDataSize);
      }
    }

    fs.close();
    IVDA_MESSAGEV("    done creating intermediate file %s", strTempMergeFilename.c_str());

    Vec3ui64 iSize = Vec3ui64(pDICOMStack->m_ivSize);
    iSize.z *= uint32_t(pDICOMStack->m_Elements.size());

    /// \todo evaluate pDICOMStack->m_strModality
    /// \todo read `is floating point' property from DICOM, instead of assuming
    /// false.
    const uint64_t timesteps = 1;
    bool result =
      RAWConverter::ConvertRAWDataset(strTempMergeFilename, strTargetFilename,
                                      strTempDir, 0, pDICOMStack->m_iAllocated,
                                      pDICOMStack->m_iComponentCount,
                                      timesteps,
                                      pDICOMStack->m_bIsBigEndian !=
                                      EndianConvert::IsBigEndian(),
                                      pDICOMStack->m_bSigned,
                                      false, iSize, pDICOMStack->m_fvfAspect,
                                      "DICOM stack",
                                      GetFilename(
                                      pDICOMStack->m_Elements[0]->m_strFileName)
                                      + " to " + GetFilename(
                                        pDICOMStack->m_Elements[
                                          pDICOMStack->m_Elements.size()-1
                                        ]->m_strFileName
                                      ),
                                      iMaxBrickSize, iBrickOverlap,
                                      m_bUseMedianFilter,
                                      m_bClampToEdge,
                                      m_iCompression,
                                      m_iCompressionLevel,
                                      m_iLayout,
                                      0, bQuantizeTo8Bit
                                     );

    if(remove(strTempMergeFilename.c_str()) != 0) {
     IVDA_WARNINGV("Unable to remove temp file %s", strTempMergeFilename.c_str());
    }

    return result;
  } else if(pStack->m_strFileType == "IMAGE") {
    IVDA_MESSAGE("  Detected Image stack, starting image conversion");
    IVDA_MESSAGEV("  Stack contains %u files",
            static_cast<unsigned>(pStack->m_Elements.size()));

    string strTempMergeFilename = strTempDir +
                                  GetFilename(strTargetFilename) +
                                  "~";
    IVDA_MESSAGEV("Creating intermediate file %s", strTempMergeFilename.c_str());

    ofstream fs;
    fs.open(strTempMergeFilename.c_str(),fstream::binary);
    if (fs.fail())  {
      IVDA_ERRORV("Could not create temp file %s aborted conversion.",
              strTempMergeFilename.c_str());
      return false;
    }

    vector<char> vData;
    for (size_t j = 0;j<pStack->m_Elements.size();j++) {
      pStack->m_Elements[j]->GetData(vData);

      fs.write(&vData[0], vData.size());
      IVDA_MESSAGEV("Creating intermediate file %s\n%u%%",
              strTempMergeFilename.c_str(),
              static_cast<unsigned>((100*j)/pStack->m_Elements.size()));
    }

    fs.close();
    IVDA_MESSAGEV("    done creating intermediate file %s",
            strTempMergeFilename.c_str());

    Vec3ui64 iSize = Vec3ui64(pStack->m_ivSize);
    iSize.z *= uint32_t(pStack->m_Elements.size());

    const string first_fn =
      GetFilename(pStack->m_Elements[0]->m_strFileName);
    const size_t last_elem = pStack->m_Elements.size()-1;
    const string last_fn =
      GetFilename(pStack->m_Elements[last_elem]->m_strFileName);

    const uint64_t timesteps = 1;

    // grab the number of components from the first file in the set.
    uint64_t components = pStack->m_Elements[0]->GetComponentCount();

    bool result =
      RAWConverter::ConvertRAWDataset(strTempMergeFilename, strTargetFilename,
                                      strTempDir, 0, pStack->m_iAllocated,
                                      components,
                                      timesteps,
                                      pStack->m_bIsBigEndian !=
                                        EndianConvert::IsBigEndian(),
                                      pStack->m_iComponentCount >= 32, false,
                                      iSize, pStack->m_fvfAspect,
                                      "Image stack",
                                      first_fn + " to " + last_fn,
                                      iMaxBrickSize, iBrickOverlap,
                                      m_bUseMedianFilter,
                                      m_bClampToEdge,
                                      m_iCompression,
                                      m_iCompressionLevel,
                                      m_iLayout);

    if(remove(strTempMergeFilename.c_str()) != 0) {
     IVDA_WARNINGV("Unable to remove temp file %s", strTempMergeFilename.c_str());
    }

    return result;
  } else {
    IVDA_ERRORV("Unknown source stack type %s", pStack->m_strFileType.c_str());
  }
  return false;
}

#ifdef DETECTED_OS_WINDOWS
  #pragma warning(default:4996)
#endif

class MergeDataset {
public:
  MergeDataset(std::string _strFilename="", uint64_t _iHeaderSkip=0, bool _bDelete=false,
               double _fScale=1.0, double _fBias=0.0) :
    strFilename(_strFilename),
    iHeaderSkip(_iHeaderSkip),
    bDelete(_bDelete),
    fScale(_fScale),
    fBias(_fBias)
  {}

  std::string strFilename;
  uint64_t iHeaderSkip;
  bool bDelete;
  double fScale;
  double fBias;
};

template <class T> class DataMerger {
public:
  DataMerger(const std::vector<MergeDataset>& strFiles,
             const std::string& strTarget, uint64_t iElemCount,
             AbstrDebugOut& dbg,
             bool bUseMaxMode) :
    bIsOK(false)
  {
    dbg.Message(_func_,"Copying first file %s ...",
                GetFilename(strFiles[0].strFilename).c_str());
    if (!LargeRAWFile::Copy(strFiles[0].strFilename, strTarget,
                            strFiles[0].iHeaderSkip)) {
      dbg.Error("Could not copy '%s' to '%s'", strFiles[0].strFilename.c_str(),
                strTarget.c_str());
      bIsOK = false;
      return;
    }

    dbg.Message(_func_,"Merging ...");
    LargeRAWFile target(strTarget);
    target.Open(true);

    if (!target.IsOpen()) {
      dbg.Error("Could not open '%s'", strTarget.c_str());
      remove(strTarget.c_str());
      bIsOK = false;
      return;
    }

    uint64_t iCopySize = std::min(iElemCount,BLOCK_COPY_SIZE/2)/sizeof(T);
    T* pTargetBuffer = new T[size_t(iCopySize)];
    T* pSourceBuffer = new T[size_t(iCopySize)];
    for (size_t i = 1;i<strFiles.size();i++) {
      dbg.Message(_func_,"Merging with file %s ...",
                  GetFilename(strFiles[i].strFilename).c_str());
      LargeRAWFile source(strFiles[i].strFilename, strFiles[i].iHeaderSkip);
      source.Open(false);
      if (!source.IsOpen()) {
        dbg.Error(_func_, "Could not open '%s'!",
                  strFiles[i].strFilename.c_str());
        delete [] pTargetBuffer;
        delete [] pSourceBuffer;
        target.Close();
        remove(strTarget.c_str());
        bIsOK = false;
        return;
      }

      uint64_t iReadSize=0;
      do {
         source.ReadRAW((unsigned char*)pSourceBuffer, iCopySize*sizeof(T));
         iCopySize = target.ReadRAW((unsigned char*)pTargetBuffer,
                                    iCopySize*sizeof(T))/sizeof(T);

         if (bUseMaxMode) {
           if (i == 1) {
             for (uint64_t j = 0;j<iCopySize;j++) {
               pTargetBuffer[j] = std::max<T>(
                 T(std::min<double>(
                   strFiles[0].fScale*(pTargetBuffer[j] + strFiles[0].fBias),
                   static_cast<double>(std::numeric_limits<T>::max())
                 )),
                 T(std::min<double>(
                   strFiles[i].fScale*(pSourceBuffer[j] + strFiles[i].fBias),
                   static_cast<double>(std::numeric_limits<T>::max()))
                 )
               );
             }
           } else {
             for (uint64_t j = 0;j<iCopySize;j++) {
               pTargetBuffer[j] = std::max<T>(
                 pTargetBuffer[j],
                 T(std::min<double>(strFiles[i].fScale*(pSourceBuffer[j] +
                                                        strFiles[i].fBias),
                                    static_cast<double>(std::numeric_limits<T>::max())))
               );
             }
           }
         } else {
           if (i == 1) {
             for (uint64_t j = 0;j<iCopySize;j++) {
               T a = T(std::min<double>(
                 strFiles[0].fScale*(pTargetBuffer[j] + strFiles[0].fBias),
                 static_cast<double>(std::numeric_limits<T>::max())
               ));
               T b = T(std::min<double>(
                 strFiles[i].fScale*(pSourceBuffer[j] + strFiles[i].fBias),
                 static_cast<double>(std::numeric_limits<T>::max())
               ));
               T val = a + b;

               if (val < a || val < b) // overflow
                 pTargetBuffer[j] = std::numeric_limits<T>::max();
               else
                 pTargetBuffer[j] = val;
             }
           } else {
             for (uint64_t j = 0;j<iCopySize;j++) {
               T b = T(std::min<double>(
                 strFiles[i].fScale*(pSourceBuffer[j] + strFiles[i].fBias),
                 static_cast<double>(std::numeric_limits<T>::max())
               ));
               T val = pTargetBuffer[j] + b;

               if (val < pTargetBuffer[j] || val < b) // overflow
                 pTargetBuffer[j] = std::numeric_limits<T>::max();
               else
                 pTargetBuffer[j] = val;
             }
           }
         }
         target.SeekPos(iReadSize*sizeof(T));
         target.WriteRAW((unsigned char*)pTargetBuffer, iCopySize*sizeof(T));
         iReadSize += iCopySize;
      } while (iReadSize < iElemCount);
      source.Close();
    }

    delete [] pTargetBuffer;
    delete [] pSourceBuffer;
    target.Close();

    bIsOK = true;
  }

  bool IsOK() const {return bIsOK;}

private:
  bool bIsOK;
};

bool IOManager::MergeDatasets(const vector <string>& strFilenames,
                              const vector <double>& vScales,
                              const vector<double>& vBiases,
                              const string& strTargetFilename,
                              const string& strTempDir,
                              bool bUseMaxMode, bool bNoUserInteraction) const {
  IVDA_MESSAGEV("Request to merge multiple data sets into %s received.",
          strTargetFilename.c_str());

  // convert the input files to RAW
  unsigned        iComponentSizeG=0;
  uint64_t        iComponentCountG=0;
  bool          bConvertEndianessG=false;
  bool          bSignedG=false;
  bool          bIsFloatG=false;
  Vec3ui64   vVolumeSizeG(0,0,0);
  Vec3f  vVolumeAspectG(0,0,0);
  string strTitleG      = "Merged data from multiple files";
  stringstream  ss;
  for (size_t i = 0;i<strFilenames.size();i++) {
    ss << GetFilename(strFilenames[i]);
    if (i<strFilenames.size()-1) ss << " ";
  }
  string        strSourceG = ss.str();

  bool bRAWCreated = false;
  vector<MergeDataset> vIntermediateFiles;
  for (size_t iInputData = 0;iInputData<strFilenames.size();iInputData++) {
    IVDA_MESSAGEV("Reading data sets %s...", strFilenames[iInputData].c_str());
    string strExt       = ToUpperCase(GetExt(strFilenames[iInputData]));

    MergeDataset IntermediateFile;
    IntermediateFile.fScale = vScales[iInputData];
    IntermediateFile.fBias = vBiases[iInputData];

    if (strExt == "UVF") {
      UVFDataset v(strFilenames[iInputData],m_iMaxBrickSize,false);

      uint64_t iLODLevel = 0; // always extract the highest quality here

      IntermediateFile.iHeaderSkip = 0;

      if (iInputData == 0)  {
        iComponentSizeG = v.GetBitWidth();
        iComponentCountG = v.GetComponentCount();
        bConvertEndianessG = !v.IsSameEndianness();
        bSignedG = v.GetIsSigned();
        bIsFloatG = v.GetIsFloat();
        vVolumeSizeG = v.GetDomainSize(static_cast<size_t>(iLODLevel));
        vVolumeAspectG = Vec3f(v.GetScale());
      } else {
#define DATA_TYPE_CHECK(a, b, errmsg) \
  do { \
    if(a != b) { \
      IVDA_ERRORV("%s", errmsg); \
      bRAWCreated = false; \
    } \
  } while(0)

        DATA_TYPE_CHECK(iComponentSizeG, v.GetBitWidth(),
                        "mismatched bit widths.");
        DATA_TYPE_CHECK(iComponentCountG, v.GetComponentCount(),
                        "different number of components.");
        DATA_TYPE_CHECK(bConvertEndianessG, !v.IsSameEndianness(),
                        "mismatched endianness.");
        DATA_TYPE_CHECK(bSignedG, v.GetIsSigned(),
                        "signedness differences");
        DATA_TYPE_CHECK(bIsFloatG, v.GetIsFloat(),
                        "some data float, other non-float.");
        DATA_TYPE_CHECK(vVolumeSizeG,
                        v.GetDomainSize(static_cast<size_t>(iLODLevel)),
                        "different volume sizes");
#undef DATA_TYPE_CHECK
        if(bRAWCreated == false) {
          IVDA_ERROR("Incompatible data types.");
          break;
        }
        if (vVolumeAspectG != Vec3f(v.GetScale()))
         IVDA_WARNING("Different aspect ratios found.");
      }

      IntermediateFile.strFilename = strTempDir + GetFilename(strFilenames[iInputData]) + ToString(rand()) +".raw";
      IntermediateFile.bDelete = true;

      if (!v.Export(iLODLevel, IntermediateFile.strFilename, false)) {
        if (FileExists(IntermediateFile.strFilename)) remove(IntermediateFile.strFilename.c_str());
        break;
      } else bRAWCreated = true;
      vIntermediateFiles.push_back(IntermediateFile);
    } else {
      unsigned      iComponentSize=0;
      uint64_t        iComponentCount=0;
      bool          bConvertEndianess=false;
      bool          bSigned=false;
      bool          bIsFloat=false;
      Vec3ui64 vVolumeSize(0,0,0);
      Vec3f  vVolumeAspect(0,0,0);
      string        strTitle = "";
      string        strSource = "";

      std::set<std::shared_ptr<AbstrConverter>> converters =
        identify_converters(strFilenames[iInputData], m_vpConverters.begin(),
                            m_vpConverters.end());
      typedef std::set<std::shared_ptr<AbstrConverter>>::const_iterator citer;
      for(citer conv = converters.begin(); conv != converters.end(); ++conv) {
        bRAWCreated = (*conv)->ConvertToRAW(
          strFilenames[iInputData], strTempDir, bNoUserInteraction,
          IntermediateFile.iHeaderSkip, iComponentSize, iComponentCount,
          bConvertEndianess, bSigned, bIsFloat, vVolumeSize, vVolumeAspect,
          strTitle, IntermediateFile.strFilename,
          IntermediateFile.bDelete
        );
        strSource = GetFilename(strFilenames[iInputData]);
        if(bRAWCreated) {
          IVDA_MESSAGEV("Conversion using '%s' succeeded!",
                  (*conv)->GetDesc().c_str());
          break;
        }
      }

      if (!bRAWCreated && (m_pFinalConverter != 0)) {
        bRAWCreated = m_pFinalConverter->ConvertToRAW(
          strFilenames[iInputData], strTempDir, bNoUserInteraction,
          IntermediateFile.iHeaderSkip, iComponentSize, iComponentCount,
          bConvertEndianess, bSigned, bIsFloat, vVolumeSize, vVolumeAspect,
          strTitle, IntermediateFile.strFilename,
          IntermediateFile.bDelete
        );
        strSource = GetFilename(strFilenames[iInputData]);
      }

      if (!bRAWCreated) {
        break;
      }

      vIntermediateFiles.push_back(IntermediateFile);

      if (iInputData == 0)  {
        iComponentSizeG = iComponentSize;
        iComponentCountG = iComponentCount;
        bConvertEndianessG = bConvertEndianess;
        bSignedG = bSigned;
        bIsFloatG = bIsFloat;
        vVolumeSizeG = vVolumeSize;
        vVolumeAspectG = vVolumeAspect;
      } else {
        if (iComponentSizeG  != iComponentSize ||
            iComponentCountG != iComponentCount ||
            bConvertEndianessG != bConvertEndianess ||
            bSignedG != bSigned ||
            bIsFloatG != bIsFloat ||
            vVolumeSizeG != vVolumeSize) {
          IVDA_ERROR("Incompatible data types.");
          bRAWCreated = false;
          break;
        }

        if (vVolumeAspectG != vVolumeAspect)
         IVDA_WARNING("Different aspect ratios found.");
      }
    }
  }

  if (!bRAWCreated) {
    IVDA_ERROR("No raw files.  Deleting temp files...");
    for (size_t i = 0;i<vIntermediateFiles.size();i++) {
      if (vIntermediateFiles[i].bDelete && FileExists(vIntermediateFiles[i].strFilename))
        remove(vIntermediateFiles[i].strFilename.c_str());
    }
    IVDA_ERROR("...  and bailing.");
    return false;
  }

  // merge the raw files into a single RAW file
  string strMergedFile = strTempDir + "merged.raw";

  bool bIsMerged = false;
  AbstrDebugOut&dbg  = *IVDA::DebugOutHandler::Instance().DebugOut();
  if (bSignedG) {
    if (bIsFloatG) {
      assert(iComponentSizeG >= 32);
      switch (iComponentSizeG) {
        case 32 : {
          DataMerger<float> d(vIntermediateFiles, strMergedFile,
                              vVolumeSizeG.volume()*iComponentCountG, dbg,
                              bUseMaxMode);
          bIsMerged = d.IsOK();
          break;
        }
        case 64 : {
          DataMerger<double> d(vIntermediateFiles, strMergedFile,
                               vVolumeSizeG.volume()*iComponentCountG, dbg,
                               bUseMaxMode);
          bIsMerged = d.IsOK();
          break;
        }
      }
    } else {
      switch (iComponentSizeG) {
        case 8  : {
          DataMerger<char> d(vIntermediateFiles, strMergedFile,
                             vVolumeSizeG.volume()*iComponentCountG, dbg,
                             bUseMaxMode);
          bIsMerged = d.IsOK();
          break;
        }
        case 16 : {
          DataMerger<short> d(vIntermediateFiles, strMergedFile,
                              vVolumeSizeG.volume()*iComponentCountG, dbg,
                              bUseMaxMode);
          bIsMerged = d.IsOK();
          break;
        }
        case 32 : {
          DataMerger<int> d(vIntermediateFiles, strMergedFile,
                            vVolumeSizeG.volume()*iComponentCountG, dbg,
                            bUseMaxMode);
          bIsMerged = d.IsOK();
          break;
        }
        case 64 : {
          DataMerger<int64_t> d(vIntermediateFiles, strMergedFile,
                                vVolumeSizeG.volume()*iComponentCountG, dbg,
                                bUseMaxMode);
          bIsMerged = d.IsOK();
          break;
        }
      }
    }
  } else {
    if (bIsFloatG) {
      // unsigned float ??? :-)
      IVDA_ERROR("Don't know how to handle unsigned float data.");
      return false;
    }
    switch (iComponentSizeG) {
      case 8  : {
        DataMerger<unsigned char> d(vIntermediateFiles, strMergedFile,
                                    vVolumeSizeG.volume()*iComponentCountG, dbg,
                                    bUseMaxMode);
        bIsMerged = d.IsOK();
        break;
      }
      case 16 : {
        DataMerger<unsigned short> d(vIntermediateFiles, strMergedFile,
                                     vVolumeSizeG.volume()*iComponentCountG, dbg,
                                     bUseMaxMode);
        bIsMerged = d.IsOK();
        break;
      }
      case 32 : {
        DataMerger<unsigned int> d(vIntermediateFiles, strMergedFile,
                                   vVolumeSizeG.volume()*iComponentCountG, dbg,
                                   bUseMaxMode);
        bIsMerged = d.IsOK();
        break;
      }
      case 64 : {
        DataMerger<uint64_t> d(vIntermediateFiles, strMergedFile,
                               vVolumeSizeG.volume()*iComponentCountG, dbg,
                               bUseMaxMode);
        bIsMerged = d.IsOK();
        break;
      }
    }
  }

  IVDA_MESSAGE("Removing temporary files...");
  for (size_t i = 0;i<vIntermediateFiles.size();i++) {
    if (vIntermediateFiles[i].bDelete && FileExists(vIntermediateFiles[i].strFilename))
      remove(vIntermediateFiles[i].strFilename.c_str());
  }
  if (!bIsMerged) {
   IVDA_WARNING("Merged failed, see other debug messages.");
    return false;
  }

  // convert that single RAW file to the target data
  string strExtTarget = ToUpperCase(GetExt(strTargetFilename));
  bool bTargetCreated = false;
  if (strExtTarget == "UVF") {
    const uint64_t timesteps = 1;
    bTargetCreated = RAWConverter::ConvertRAWDataset(
        strMergedFile, strTargetFilename, strTempDir, 0,
        iComponentSizeG, iComponentCountG, timesteps, bConvertEndianessG,
        bSignedG, bIsFloatG, vVolumeSizeG, vVolumeAspectG, strTitleG,
        GetFilename(strMergedFile), m_iMaxBrickSize,
        m_iBrickOverlap, m_bUseMedianFilter, m_bClampToEdge, m_iCompression,
        m_iCompressionLevel, m_iLayout);
  } else {
    for (size_t k = 0;k<m_vpConverters.size();k++) {
      const vector<string>& vStrSupportedExtTarget =
        m_vpConverters[k]->SupportedExt();
      for (size_t l = 0;l<vStrSupportedExtTarget.size();l++) {
        if (vStrSupportedExtTarget[l] == strExtTarget) {
          bTargetCreated = m_vpConverters[k]->ConvertToNative(
            strMergedFile, strTargetFilename, 0, iComponentSizeG,
            iComponentCountG, bSignedG, bIsFloatG, vVolumeSizeG,
            vVolumeAspectG, bNoUserInteraction, false
          );

          if(!bTargetCreated) {
           IVDA_WARNINGV("%s said it could convert to native, but failed!",
                    m_vpConverters[k]->GetDesc().c_str());
          } else {
            break;
          }
        }
      }
      if (bTargetCreated) break;
    }
  }
  remove(strMergedFile.c_str());
  return bTargetCreated;
}


bool IOManager::ConvertDataset(const string& strFilename,
                               const string& strTargetFilename,
                               const string& strTempDir,
                               const bool bNoUserInteraction,
                               const uint64_t iMaxBrickSize,
                               const uint64_t iBrickOverlap,
                               const bool bQuantizeTo8Bit) const {
  list<string> files;
  files.push_back(strFilename);
  return ConvertDataset(files, strTargetFilename, strTempDir,
                        bNoUserInteraction, iMaxBrickSize, iBrickOverlap,
                        bQuantizeTo8Bit);
}

bool IOManager::ConvertDataset(const list<string>& files,
                               const string& strTargetFilename,
                               const string& strTempDir,
                               const bool bNoUserInteraction,
                               const uint64_t iMaxBrickSize,
                               uint64_t iBrickOverlap,
                               bool bQuantizeTo8Bit) const
{
  if(files.empty()) {
    IVDA_ERROR("No files to convert?!");
    return false;
  }
  {
    ostringstream request;
    request << "Request to convert datasets ";
    copy(files.begin(), files.end(), ostream_iterator<string>(request, ", "));
    request << "to " << strTargetFilename << " received.";
    IVDA_MESSAGEV("%s", request.str().c_str());
  }

  // this might actually be a valid test case, if you want to compare
  // performance across brick sizes.  However it's completely ridiculous in
  // actual use, and catches a confusing bug if you forget an argument in the
  // API call (which still compiles due to default arguments!).
  assert(iMaxBrickSize >= 8 &&
         "Incredibly small bricks -- are you sure?");

  /// @todo verify the list of files is `compatible':
  ///   dimensions are the same
  ///   all from the same file format
  ///   all have equivalent bit depth, or at least something that'll convert to
  ///    the same depth
  string strExt = ToUpperCase(GetExt(*files.begin()));
  string strExtTarget = ToUpperCase(GetExt(strTargetFilename));

  if (strExtTarget == "UVF") {
    // Iterate through all our converters, stopping when one successfully
    // converts our data.
    std::set<std::shared_ptr<AbstrConverter>> converters =
      identify_converters(*files.begin(), m_vpConverters.begin(),
                          m_vpConverters.end());
    typedef std::set<std::shared_ptr<AbstrConverter>>::const_iterator citer;
    for(citer conv = converters.begin(); conv != converters.end(); ++conv) {
      if (!(*conv)->CanImportData()) continue;

      if((*conv)->ConvertToUVF(files, strTargetFilename, strTempDir,
                               bNoUserInteraction, iMaxBrickSize, iBrickOverlap,
                               m_bUseMedianFilter, m_bClampToEdge,
                               m_iCompression, m_iCompressionLevel, m_iLayout,
                               bQuantizeTo8Bit)) {
        return true;
      } else {
       IVDA_WARNINGV("Converter %s can read files, but conversion failed!",
                (*conv)->GetDesc().c_str());
      }
    }

    IVDA_MESSAGE("No suitable automatic converter found!");

    if (m_pFinalConverter != 0) {
      IVDA_MESSAGE("Attempting fallback converter.");
      return m_pFinalConverter->ConvertToUVF(files, strTargetFilename,
                                             strTempDir, bNoUserInteraction,
                                             iMaxBrickSize, iBrickOverlap,
                                             m_bUseMedianFilter, m_bClampToEdge,
                                             m_iCompression,
                                             m_iCompressionLevel,
                                             m_iLayout,
                                             bQuantizeTo8Bit);
    } else {
      return false;
    }
  }

  if(files.size() > 1) {
    IVDA_ERROR("Cannot convert multiple files to anything but UVF.");
    return false;
  }
  // Everything below is for exporting to non-UVF formats.

  string   strFilename = *files.begin();
  uint64_t        iHeaderSkip=0;
  unsigned        iComponentSize=0;
  uint64_t        iComponentCount=0;
  bool          bConvertEndianess=false;
  bool          bSigned=false;
  bool          bIsFloat=false;
  Vec3ui64 vVolumeSize(0,0,0);
  Vec3f  vVolumeAspect(0,0,0);
  string        strTitle = "";
  string        strSource = "";
  string        strIntermediateFile = "";
  bool          bDeleteIntermediateFile = false;

  bool bRAWCreated = false;

  // source is UVF
  if (strExt == "UVF") {
    // max(): disable bricksize check
    UVFDataset v(strFilename,numeric_limits<uint64_t>::max(),false,false);

    uint64_t iLODLevel = 0; // always extract the highest quality here

    iHeaderSkip = 0;
    iComponentSize = v.GetBitWidth();
    iComponentCount = v.GetComponentCount();
    bConvertEndianess = !v.IsSameEndianness();
    bSigned = v.GetIsSigned();
    bIsFloat = v.GetIsFloat();
    vVolumeSize = v.GetDomainSize(static_cast<size_t>(iLODLevel));
    vVolumeAspect = Vec3f(v.GetScale());
    strTitle          = "UVF data";               /// \todo grab this data from the UVF file
    strSource         = GetFilename(strFilename);

    strIntermediateFile = strTempDir + strSource +".raw";
    bDeleteIntermediateFile = true;

    if (!v.Export(iLODLevel, strIntermediateFile, false)) {
      if (FileExists(strIntermediateFile)) {
        RAWConverter::Remove(strIntermediateFile.c_str(),
                         *IVDA::DebugOutHandler::Instance().DebugOut());
      }
      return false;
    } else bRAWCreated = true;
  } else { // for non-UVF source data
    vector<int8_t> bytes(512);
    read_first_block(strFilename, bytes);

    std::set<std::shared_ptr<AbstrConverter>> converters =
      identify_converters(*files.begin(), m_vpConverters.begin(),
                          m_vpConverters.end());
    typedef std::set<std::shared_ptr<AbstrConverter>>::const_iterator citer;
    for(citer conv = converters.begin(); conv != converters.end(); ++conv) {
      if((*conv)->ConvertToRAW(strFilename, strTempDir,
                               bNoUserInteraction,iHeaderSkip,
                               iComponentSize, iComponentCount,
                               bConvertEndianess, bSigned,
                               bIsFloat, vVolumeSize,
                               vVolumeAspect, strTitle,
                               strIntermediateFile,
                               bDeleteIntermediateFile)) {
        bRAWCreated = true;
        break;
      }
    }

    if (!bRAWCreated && (m_pFinalConverter != 0)) {
      IVDA_MESSAGE("No converter can read the data.  Trying fallback converter.");
      bRAWCreated = m_pFinalConverter->ConvertToRAW(strFilename, strTempDir,
                                                    bNoUserInteraction,
                                                    iHeaderSkip,
                                                    iComponentSize,
                                                    iComponentCount,
                                                    bConvertEndianess, bSigned,
                                                    bIsFloat, vVolumeSize,
                                                    vVolumeAspect,
                                                    strTitle,
                                                    strIntermediateFile,
                                                    bDeleteIntermediateFile);
    }
  }
  if (!bRAWCreated) { return false; }

  bool bTargetCreated = false;
  for (size_t k = 0;k<m_vpConverters.size();k++) {
    const vector<string>& vStrSupportedExtTarget =
      m_vpConverters[k]->SupportedExt();
    for (size_t l = 0;l<vStrSupportedExtTarget.size();l++) {
      if (vStrSupportedExtTarget[l] == strExtTarget) {
        bTargetCreated =
          m_vpConverters[k]->ConvertToNative(strIntermediateFile,
                                             strTargetFilename,
                                             iHeaderSkip, iComponentSize,
                                             iComponentCount, bSigned,
                                             bIsFloat, vVolumeSize,
                                             vVolumeAspect,
                                             bNoUserInteraction, bQuantizeTo8Bit);
        if (bTargetCreated) break;
      }
    }
    if (bTargetCreated) break;
  }
  if (bDeleteIntermediateFile) remove(strIntermediateFile.c_str());
  if (bTargetCreated) return true;

  return false;
}

void IOManager::SetMemManLoadFunction(
  std::function<Tuvok::Dataset*(const std::string&,
                                     AbstrRenderer*)>& f
) {
  m_LoadDS = f;
}

Dataset* IOManager::LoadDataset(const string& strFilename,
                                AbstrRenderer* requester) const {
  if(!m_LoadDS) {
    // logic error; you should have set this after creating the MemMgr!
    IVDA_ERROR("Never set the internal LoadDS callback!");
    throw Tuvok::io::DSOpenFailed("Internal error; callback never set!",
                                  __FILE__, __LINE__);
  }
  return m_LoadDS(strFilename, requester);
}

Dataset* IOManager::LoadRebrickedDataset(const std::string& filename,
                                         const Vec3ui bricksize,
                                         size_t minmaxType) const {
  std::shared_ptr<Dataset> ds(this->CreateDataset(filename, 1024, false));
  std::shared_ptr<LinearIndexDataset> lid =
    std::dynamic_pointer_cast<LinearIndexDataset>(ds);
  if(!lid) {
    IVDA_ERROR("Can only rebrick a LinearIndexDataset, sorry.");
    return NULL;
  }
  if(minmaxType > DynamicBrickingDS::MM_DYNAMIC) {
    throw std::logic_error("minmaxType too large");
  }
  if(bricksize.volume() == 0) { IVDA_ERROR("null brick size"); return NULL; }

  // make sure the subdivision works; we need to be able to fit bricks within
  // the source bricks.  but make sure not to include ghost data when we
  // calculate that!
  const Vec3ui overlap = lid->GetBrickOverlapSize() * 2;
  const Vec3ui src_bsize = lid->GetMaxBrickSize();
  std::array<size_t,3> tgt_bsize = {{
    std::min(bricksize[0], src_bsize[0]),
    std::min(bricksize[1], src_bsize[1]),
    std::min(bricksize[2], src_bsize[2])
  }};
  for(unsigned i=0; i < 3; ++i) {
    if(((src_bsize[i]-overlap[i]) % (tgt_bsize[i]-overlap[i])) != 0) {
      IVDA_ERRORV("%u dimension target brick size (%u) is not a multiple of source "
              "brick size (%u)", i, tgt_bsize[i] - overlap[i],
              src_bsize[i] - overlap[i]);
      return NULL;
    }
  }

  const size_t cache_size = static_cast<size_t>(
    0.80f * BasicsController::ConstInstance().SysInfo().GetMaxUsableCPUMem()
  );
  enum DynamicBrickingDS::MinMaxMode mm =
    static_cast<enum DynamicBrickingDS::MinMaxMode>(minmaxType);
  return new DynamicBrickingDS(lid, tgt_bsize, cache_size, mm);
}

Dataset* IOManager::CreateDataset(const string& filename,
                                  uint64_t max_brick_size, bool verify) const {
  IVDA_MESSAGEV("Searching for appropriate DS for '%s'", filename.c_str());
  return m_dsFactory->Create(filename, max_brick_size, verify);
}

void IOManager::AddReader(shared_ptr<FileBackedDataset> ds) {
  m_dsFactory->AddReader(ds);
}

class MCData {
public:
  MCData(const std::string& strTargetFile) :
    m_strTargetFile(strTargetFile)
  {}

  virtual ~MCData() {}
  virtual bool PerformMC(void* pData, const Vec3ui& vBrickSize, const Vec3ui64& vBrickOffset) = 0;

protected:
  std::string m_strTargetFile;
};

bool MCBrick(void* pData, const Vec3ui64& vBrickSize,
             const Vec3ui64& vBrickOffset, void* pUserContext) {
    MCData* pMCData = (MCData*)pUserContext;
    return pMCData->PerformMC(pData, Vec3ui(vBrickSize), vBrickOffset);
}


bool IOManager::ExtractImageStack(const Tuvok::UVFDataset* pSourceData,
                                  const DataIO::TransferFunction1D* pTrans,
                                  uint64_t iLODlevel,
                                  const std::string& strTargetFilename,
                                  const std::string& strTempDir,
                                  bool bAllDirs) const {


  string strTempFilename = FindNextSequenceName(strTempDir + GetFilename(strTargetFilename)+".tmp_raw");

  if (pSourceData->GetIsFloat() || pSourceData->GetIsSigned()) {
    IVDA_ERROR("Stack export currently only supported for unsigned integer values.");
    return false;
  }

  if (pSourceData->GetComponentCount() > 4) {
    IVDA_ERROR("Only up to four component data supported");
    return false;
  }


  IVDA_MESSAGE("Extracting Data");

  bool bRAWCreated = pSourceData->Export(iLODlevel, strTempFilename, false);

  if (!bRAWCreated) {
    IVDA_ERRORV("Unable to write temp file %s", strTempFilename.c_str());
    return false;
  }

  IVDA_MESSAGE("Writing stacks");

  double fMaxActValue = (pSourceData->GetRange().first > pSourceData->GetRange().second) ? pTrans->GetSize() : pSourceData->GetRange().second;

  bool bTargetCreated = StackExporter::WriteStacks(strTempFilename,
                                                   strTargetFilename,
                                                   pTrans,
                                                   pSourceData->GetBitWidth(),
                                                   pSourceData->GetComponentCount(),
                                                   float(pTrans->GetSize() / fMaxActValue),
                                                   pSourceData->GetDomainSize(static_cast<size_t>(iLODlevel)),
                                                   bAllDirs);
  remove(strTempFilename.c_str());

  if (!bTargetCreated) {
    IVDA_ERRORV("Unable to write target file %s", strTargetFilename.c_str());
    return false;
  }

  IVDA_MESSAGE("Done!");

  return bTargetCreated;
}

template <class T> class MCDataTemplate  : public MCData {
public:
  MCDataTemplate(const std::string& strTargetFile, T TIsoValue,
                 const Vec3f& vScale,
                 Vec3ui64 vDataSize, Tuvok::AbstrGeoConverter* conv,
                 const Vec4f& vColor) :
    MCData(strTargetFile),
    m_TIsoValue(TIsoValue),
    m_iIndexoffset(0),
    m_pMarchingCubes(new MarchingCubes<T>()),
    m_vDataSize(vDataSize),
    m_conv(conv),
    m_vColor(vColor),
    m_vScale(vScale)
  {
  }

  virtual ~MCDataTemplate() {
    Tuvok::Mesh m = Tuvok::Mesh(m_vertices, m_normals, Tuvok::TexCoordVec(),
                                Tuvok::ColorVec(), m_indices, m_indices,
                                Tuvok::IndexVec(),Tuvok::IndexVec(),
                                false,false,"Marching Cubes mesh by ImageVis3D",
                                Tuvok::Mesh::MT_TRIANGLES);
    m.SetDefaultColor(m_vColor);
    m_conv->ConvertToNative(m, m_strTargetFile);
  }

  virtual bool PerformMC(void* pData, const Vec3ui& vBrickSize, const Vec3ui64& vBrickOffset) {

    T* ptData = (T*)pData;

    // extract isosurface
    m_pMarchingCubes->SetVolume(vBrickSize.x, vBrickSize.y, vBrickSize.z,
                                ptData);
    m_pMarchingCubes->Process(m_TIsoValue);

    // brick scale
    float fMaxSize = (Vec3f(m_vDataSize) * m_vScale).maxVal();

    Vec3f vecBrickOffset(vBrickOffset);
    vecBrickOffset = vecBrickOffset * m_vScale;

    for (int i = 0;i<m_pMarchingCubes->m_Isosurface->iVertices;i++) {
      m_vertices.push_back((m_pMarchingCubes->m_Isosurface->vfVertices[i]+vecBrickOffset-Vec3f(m_vDataSize)/2.0f)/fMaxSize);
    }

    for (int i = 0;i<m_pMarchingCubes->m_Isosurface->iVertices;i++) {
      m_normals.push_back(m_pMarchingCubes->m_Isosurface->vfNormals[i]);
    }

    for (int i = 0;i<m_pMarchingCubes->m_Isosurface->iTriangles;i++) {
      m_indices.push_back(m_pMarchingCubes->m_Isosurface->viTriangles[i].x+m_iIndexoffset);
      m_indices.push_back(m_pMarchingCubes->m_Isosurface->viTriangles[i].y+m_iIndexoffset);
      m_indices.push_back(m_pMarchingCubes->m_Isosurface->viTriangles[i].z+m_iIndexoffset);
    }

    m_iIndexoffset += m_pMarchingCubes->m_Isosurface->iVertices;

    return true;
  }

protected:
  T                  m_TIsoValue;
  uint32_t           m_iIndexoffset;
  std::shared_ptr<MarchingCubes<T>> m_pMarchingCubes;
  Vec3ui64      m_vDataSize;
  Tuvok::AbstrGeoConverter* m_conv;
  Vec4f       m_vColor;
  Vec3f       m_vScale;
  Tuvok::VertVec     m_vertices;
  Tuvok::NormVec     m_normals;
  Tuvok::IndexVec    m_indices;
};

bool IOManager::ExtractIsosurface(const Tuvok::UVFDataset* pSourceData,
                                  uint64_t iLODlevel, double fIsovalue,
                                  const Vec4f& vfColor,
                                  const string& strTargetFilename,
                                  const string& strTempDir) const {
  if (pSourceData->GetComponentCount() != 1) {
    IVDA_ERROR("Isosurface extraction only supported for scalar volumes.");
    return false;
  }

  string strTempFilename = strTempDir + GetFilename(strTargetFilename)+".tmp_raw";
  std::shared_ptr<MCData> pMCData;

  bool   bFloatingPoint  = pSourceData->GetIsFloat();
  bool   bSigned         = pSourceData->GetIsSigned();
  unsigned iComponentSize = pSourceData->GetBitWidth();
  Vec3f vScale    = Vec3f(pSourceData->GetScale());

  AbstrGeoConverter* conv = GetGeoConverterForExt(ToLowerCase(GetExt(strTargetFilename)),true, false);

  if (conv == NULL) {
    IVDA_ERROR("Unknown Mesh Format.");
    return false;
  }

  Vec3ui64 vDomainSize = pSourceData->GetDomainSize(size_t(iLODlevel));

  if (bFloatingPoint) {
    if (bSigned) {
      switch (iComponentSize) {
        case 32:
          pMCData.reset(new MCDataTemplate<float>(strTargetFilename,
            float(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 64:
          pMCData.reset(new MCDataTemplate<double>(strTargetFilename,
            double(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
      }
    }
  } else {
    if (bSigned) {
      switch (iComponentSize) {
        case  8:
          pMCData.reset(new MCDataTemplate<char>(strTargetFilename,
            char(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 16:
          pMCData.reset(new MCDataTemplate<short>(strTargetFilename,
            short(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 32:
          pMCData.reset(new MCDataTemplate<int>(strTargetFilename,
            int(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 64:
          pMCData.reset(new MCDataTemplate<int64_t>(strTargetFilename,
            int64_t(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
      }
    } else {
      switch (iComponentSize) {
        case  8:
          pMCData.reset(new MCDataTemplate<unsigned char>(strTargetFilename,
            (unsigned char)(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 16:
          pMCData.reset(new MCDataTemplate<unsigned short>(strTargetFilename,
            (unsigned short)(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 32:
          pMCData.reset(new MCDataTemplate<uint32_t>(strTargetFilename,
            uint32_t(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
        case 64:
          pMCData.reset(new MCDataTemplate<uint64_t>(strTargetFilename,
            uint64_t(fIsovalue), vScale, vDomainSize, conv, vfColor
          )); break;
      }
    }
  }

  if (!pMCData) {
    IVDA_ERROR("Unsupported data format.");
    return false;
  }

  bool bResult = pSourceData->ApplyFunction(iLODlevel,&MCBrick,
                                            (void*)pMCData.get(), 1);

  if (FileExists(strTempFilename)) remove (strTempFilename.c_str());

  if (bResult)
    return true;
  else {
    remove (strTargetFilename.c_str());
    IVDA_ERROR("Export call failed.");
    return false;
  }
}

bool IOManager::ExportMesh(const std::shared_ptr<Mesh> mesh,
                           const std::string& strTargetFilename) {
  AbstrGeoConverter* conv = GetGeoConverterForExt(ToLowerCase(GetExt(strTargetFilename)),true, false);

  if (conv == NULL) {
    IVDA_ERROR("Unknown Mesh Format.");
    return false;
  }

  return conv->ConvertToNative(*mesh, strTargetFilename);
}

bool IOManager::ExportDataset(const UVFDataset* pSourceData, uint64_t iLODlevel,
                              const string& strTargetFilename,
                              const string& strTempDir) const {
  // find the right converter to handle the output
  string strExt = ToUpperCase(GetExt(strTargetFilename));
  std::shared_ptr<AbstrConverter> pExporter;
  for (size_t i = 0;i<m_vpConverters.size();i++) {
    const vector<string>& vStrSupportedExt = m_vpConverters[i]->SupportedExt();
    for (size_t j = 0;j<vStrSupportedExt.size();j++) {
      if (vStrSupportedExt[j] == strExt) {
        pExporter = m_vpConverters[i];
        break;
      }
    }
    if (pExporter) break;
  }

  if (!pExporter) {
    IVDA_ERRORV("Unknown file extension %s.", strExt.c_str());
    return false;
  }

  string strTempFilename = strTempDir + GetFilename(strTargetFilename)+".tmp_raw";
  bool bRAWCreated = pSourceData->Export(iLODlevel, strTempFilename, false);

  if (!bRAWCreated) {
    IVDA_ERRORV("Unable to write temp file %s", strTempFilename.c_str());
    return false;
  }

  IVDA_MESSAGE("Writing Target Dataset");

  bool bTargetCreated = false;

  try {
    bTargetCreated = pExporter->ConvertToNative(
                      strTempFilename, strTargetFilename, 0,
                      pSourceData->GetBitWidth(),
                      pSourceData->GetComponentCount(),
                      pSourceData->GetIsSigned(),
                      pSourceData->GetIsFloat(),
                      pSourceData->GetDomainSize(static_cast<size_t>(iLODlevel)),
                      Vec3f(pSourceData->GetScale()),
                      false, false
                    );
    remove(strTempFilename.c_str());
  } catch (const Tuvok::io::DSOpenFailed& err) {
    IVDA_ERRORV("Unable to write target file %s", err.what());
    return false;
  }

  if (!bTargetCreated) {
    IVDA_ERRORV("Unable to write target file %s", strTargetFilename.c_str());
    return false;
  }

  IVDA_MESSAGE("Done!");

  return bTargetCreated;
}


// Try to find the reader for the filename.  If we get back garbage, that must
// mean we can't read this.  If we can't read it, it needs to be converted.
// All your data are belong to us.
bool IOManager::NeedsConversion(const string& strFilename) const {
  const weak_ptr<FileBackedDataset> reader = m_dsFactory->Reader(strFilename);
  return reader.expired();
}

// Some readers checksum the data.  If they do, this is how the UI will access
// that verification method.
bool IOManager::Verify(const string& strFilename) const
{
  const weak_ptr<FileBackedDataset> reader = m_dsFactory->Reader(strFilename);

  // I swear I did not purposely choose words so that this text aligned.
  assert(!reader.expired() && "Impossible; we wouldn't have reached this code "
                              "unless we thought that the format doesn't need "
                              "conversion.  But we only think it doesn't need "
                              "conversion when there's a known reader for the "
                              "file.");

  // Upcast it.  Hard to verify a checksum on an abstract entity.
  const shared_ptr<FileBackedDataset> fileds =
    dynamic_pointer_cast<FileBackedDataset>(reader.lock());

  return fileds->Verify(strFilename);
}

using namespace Tuvok;
using namespace Tuvok::io;

std::string IOManager::GetImageExportDialogString() const {
  std::vector<std::pair<std::string,std::string>> formats = StackExporter::GetSuportedImageFormats();

  string strDialog = "All known Files ( ";
  for(size_t i = 0; i< formats.size(); ++i) {
    strDialog += "*." + ToLowerCase(formats[i].first) + " ";
  }
  strDialog += ");;";

  for(size_t i = 0; i< formats.size(); ++i) {
    strDialog += formats[i].second + " (*." + ToLowerCase(formats[i].first) + ");;";
  }

  return strDialog;
}

std::string IOManager::ImageExportDialogFilterToExt(const string& filter) const {
  std::vector<std::pair<std::string,std::string>> formats = StackExporter::GetSuportedImageFormats();

  for(size_t i = 0; i< formats.size(); ++i) {
    std::string strDialog = formats[i].second + " (*." + ToLowerCase(formats[i].first) + ")";
    if ( filter == strDialog )
      return ToLowerCase(formats[i].first);
  }
  return "";
}


string IOManager::GetLoadDialogString() const {
  string strDialog = "All known Files (";
  map<string,string> descPairs;

  // first create the show all text entry
  // native formats
  const DSFactory::DSList& readers = m_dsFactory->Readers();
  for(DSFactory::DSList::const_iterator rdr=readers.begin();
      rdr != readers.end(); ++rdr) {
    const shared_ptr<FileBackedDataset> fileds =
      dynamic_pointer_cast<FileBackedDataset>(*rdr);
    const list<string> extensions = fileds->Extensions();
    for(list<string>::const_iterator ext = extensions.begin();
        ext != extensions.end(); ++ext) {
      strDialog += "*." + ToLowerCase(*ext) + " ";
      descPairs[*ext] = (*rdr)->Name();
    }
  }

  // converters
  for (size_t i = 0;i<m_vpConverters.size();i++) {
    if (m_vpConverters[i]->CanImportData()) {
      for (size_t j = 0;j<m_vpConverters[i]->SupportedExt().size();j++) {
        string strExt = ToLowerCase(m_vpConverters[i]->SupportedExt()[j]);
        if (descPairs.count(strExt) == 0) {
          strDialog = strDialog + "*." + strExt + " ";
          descPairs[strExt] = m_vpConverters[i]->GetDesc();
        }
      }
    }
  }
  strDialog += ");;";

  // now create the separate entries, i.e. just UVFs, just TIFFs, etc.
  // native formats
  for(DSFactory::DSList::const_iterator rdr=readers.begin();
      rdr != readers.end(); ++rdr) {
    const shared_ptr<FileBackedDataset> fileds =
      dynamic_pointer_cast<FileBackedDataset>(*rdr);
    const list<string> extensions = fileds->Extensions();
    strDialog += string(fileds->Name()) + " (";
    for(list<string>::const_iterator ext = extensions.begin();
        ext != extensions.end(); ++ext) {
      strDialog += "*." + ToLowerCase(*ext) + " ";
      descPairs[*ext] = (*rdr)->Name();
    }
    strDialog += ");;";
  }

  // converters
  for (size_t i=0; i < m_vpConverters.size(); i++) {
    if (m_vpConverters[i]->CanImportData()) {
      strDialog += m_vpConverters[i]->GetDesc() + " (";
      for (size_t j=0; j < m_vpConverters[i]->SupportedExt().size(); j++) {
        string strExt = ToLowerCase(m_vpConverters[i]->SupportedExt()[j]);
        strDialog += "*." + strExt;
        if (j<m_vpConverters[i]->SupportedExt().size()-1)
          strDialog += " ";
      }
      strDialog += ");;";
    }
  }

  strDialog += "All Files (*)";

  return strDialog;
}

string IOManager::GetExportDialogString() const {
  string strDialog;
  // separate entries
  for (size_t i=0; i < m_vpConverters.size(); i++) {
    if (m_vpConverters[i]->CanExportData()) {
      for (size_t j=0; j < m_vpConverters[i]->SupportedExt().size(); j++) {
        string strExt = ToLowerCase(m_vpConverters[i]->SupportedExt()[j]);
        strDialog += m_vpConverters[i]->GetDesc() + " (*." + strExt + ");;";
      }
    }
  }

  return strDialog;
}


std::string IOManager::ExportDialogFilterToExt(const string& filter) const {
  std::vector<std::pair<std::string,std::string>> formats = StackExporter::GetSuportedImageFormats();

  for(size_t i = 0; i< formats.size(); ++i) {
    if (m_vpConverters[i]->CanExportData()) {
      for (size_t j=0; j < m_vpConverters[i]->SupportedExt().size(); j++) {
        string strExt = ToLowerCase(m_vpConverters[i]->SupportedExt()[j]);
        std::string  strDialog = m_vpConverters[i]->GetDesc() + " (*." + strExt + ")";
        if ( filter == strDialog )
          return ToLowerCase(strExt);
      }
    }
  }
  return "";
}


vector<pair<string, string>> IOManager::GetExportFormatList() const {
  vector<pair<string, string>> v;
  v.push_back(make_pair("UVF", "Universal Volume Format"));
  for (size_t i = 0;i<m_vpConverters.size();i++) {
    if (m_vpConverters[i]->CanExportData()) {
      for (size_t j = 0;j<m_vpConverters[i]->SupportedExt().size();j++) {
        v.push_back(
          make_pair(ToLowerCase(m_vpConverters[i]->SupportedExt()[j]),
                    m_vpConverters[i]->GetDesc()));
      }
    }
  }
  return v;
}

vector<pair<string, string>> IOManager::GetImportFormatList() const {
  vector<pair<string, string>> v;
  v.push_back(make_pair("UVF", "Universal Volume Format"));
  for (size_t i = 0;i<m_vpConverters.size();i++) {
    if (m_vpConverters[i]->CanImportData()) {
      for (size_t j = 0;j<m_vpConverters[i]->SupportedExt().size();j++) {
        v.push_back(
          make_pair(ToLowerCase(m_vpConverters[i]->SupportedExt()[j]),
                    m_vpConverters[i]->GetDesc()));
      }
    }
  }
  return v;
}


vector< tConverterFormat > IOManager::GetFormatList() const {

  vector< tConverterFormat > v;
  v.push_back(make_tuple("UVF", "Universal Volume Format", true, true));
  for (size_t i = 0;i<m_vpConverters.size();i++) {
      for (size_t j = 0;j<m_vpConverters[i]->SupportedExt().size();j++) {
        v.push_back(make_tuple(
                        ToLowerCase(
                          m_vpConverters[i]->SupportedExt()[j]
                        ),
                        m_vpConverters[i]->GetDesc(),
                        m_vpConverters[i]->CanExportData(),
                        m_vpConverters[i]->CanImportData()));
    }
  }
  return v;
}

std::shared_ptr<AbstrConverter> IOManager::GetConverterForExt(std::string ext,
                                              bool bMustSupportExport,
                                              bool bMustSupportImport) const {
  for (size_t i = 0;i<m_vpConverters.size();i++) {
    if ((!bMustSupportExport || m_vpConverters[i]->CanExportData()) &&
        (!bMustSupportImport || m_vpConverters[i]->CanImportData())) {
      for (size_t j = 0;j<m_vpConverters[i]->SupportedExt().size();j++) {
        string convExt = ToLowerCase(m_vpConverters[i]->SupportedExt()[j]);
        if (ext == convExt) return m_vpConverters[i];
      }
    }
  }
  return NULL;
}


AbstrGeoConverter* IOManager::GetGeoConverterForExt(std::string ext,
                                                    bool bMustSupportExport,
                                                    bool bMustSupportImport) const {
  for (size_t i = 0;i<m_vpGeoConverters.size();i++) {
    if ((!bMustSupportExport || m_vpGeoConverters[i]->CanExportData()) &&
        (!bMustSupportImport || m_vpGeoConverters[i]->CanImportData())) {
      for (size_t j = 0;j<m_vpGeoConverters[i]->SupportedExt().size();j++) {
        string convExt = ToLowerCase(m_vpGeoConverters[i]->SupportedExt()[j]);
        if (ext == convExt) return m_vpGeoConverters[i];
      }
    }
  }
  return NULL;
}

string IOManager::GetLoadGeoDialogString() const {
  string strDialog = "All known Geometry Files (";
  map<string,string> descPairs;

  // converters
  for (size_t i = 0;i<m_vpGeoConverters.size();i++) {
    if (m_vpGeoConverters[i]->CanImportData()) {
      for (size_t j = 0;j<m_vpGeoConverters[i]->SupportedExt().size();j++) {
        string strExt = ToLowerCase(m_vpGeoConverters[i]->SupportedExt()[j]);
        if (descPairs.count(strExt) == 0) {
          strDialog = strDialog + "*." + strExt + " ";
          descPairs[strExt] = m_vpGeoConverters[i]->GetDesc();
        }
      }
    }
  }
  strDialog += ");;";

  // now create the separate entries, i.e. just OBJs, TRIs, etc.
  for (size_t i=0; i < m_vpGeoConverters.size(); i++) {
    if (m_vpGeoConverters[i]->CanImportData()) {
      strDialog += m_vpGeoConverters[i]->GetDesc() + " (";
      for (size_t j=0; j < m_vpGeoConverters[i]->SupportedExt().size(); j++) {
        string strExt = ToLowerCase(m_vpGeoConverters[i]->SupportedExt()[j]);
        strDialog += "*." + strExt;
        if (j<m_vpGeoConverters[i]->SupportedExt().size()-1)
          strDialog += " ";
      }
      strDialog += ");;";
    }
  }

  strDialog += "All Files (*)";

  return strDialog;
}

string IOManager::GetGeoExportDialogString() const {
  string strDialog;
  // separate entries
  for (size_t i=0; i < m_vpGeoConverters.size(); i++) {
    if (m_vpGeoConverters[i]->CanExportData()) {
      for (size_t j=0; j < m_vpGeoConverters[i]->SupportedExt().size(); j++) {
        string strExt = ToLowerCase(m_vpGeoConverters[i]->SupportedExt()[j]);
        strDialog += m_vpGeoConverters[i]->GetDesc() + " (*." + strExt + ");;";
      }
    }
  }

  return strDialog;
}



vector<pair<string, string>> IOManager::GetGeoExportFormatList() const {
  vector<pair<string, string>> v;
  for (size_t i = 0;i<m_vpGeoConverters.size();i++) {
    for (size_t j = 0;j<m_vpGeoConverters[i]->SupportedExt().size();j++) {
      if (m_vpGeoConverters[i]->CanExportData()) {
        v.push_back(
          make_pair(ToLowerCase(m_vpGeoConverters[i]->SupportedExt()[j]),
                    m_vpGeoConverters[i]->GetDesc()));
      }
    }
  }
  return v;
}

vector<pair<string, string>> IOManager::GetGeoImportFormatList() const {
  vector<pair<string, string>> v;
  for (size_t i = 0;i<m_vpGeoConverters.size();i++) {
    if (m_vpGeoConverters[i]->CanImportData()) {
      for (size_t j = 0;j<m_vpGeoConverters[i]->SupportedExt().size();j++) {
        v.push_back(
          make_pair(ToLowerCase(m_vpGeoConverters[i]->SupportedExt()[j]),
                    m_vpGeoConverters[i]->GetDesc()));
      }
    }
  }
  return v;
}


vector< tConverterFormat > IOManager::GetGeoFormatList() const {
  vector< tConverterFormat > v;
  for (size_t i = 0;i<m_vpGeoConverters.size();i++) {
    for (size_t j = 0;j<m_vpGeoConverters[i]->SupportedExt().size();j++) {
      v.push_back(make_tuple(
                      ToLowerCase(
                        m_vpGeoConverters[i]->SupportedExt()[j]
                      ),
                      m_vpGeoConverters[i]->GetDesc(),
                      m_vpGeoConverters[i]->CanExportData(),
                      m_vpGeoConverters[i]->CanImportData()));
    }
  }
  return v;
}

bool IOManager::AnalyzeDataset(const string& strFilename, RangeInfo& info,
                               const string& strTempDir) const {
  // find the right converter to handle the dataset
  string strExt = ToUpperCase(GetExt(strFilename));

  if (strExt == "UVF") {
    UVFDataset v(strFilename,m_iMaxBrickSize,false);

    uint64_t iComponentCount = v.GetComponentCount();
    bool bSigned = v.GetIsSigned();
    bool bIsFloat = v.GetIsFloat();

    if (iComponentCount != 1) return false;  // only scalar data supported at the moment

    info.m_fRange.first = v.GetRange().first;
    info.m_fRange.second = v.GetRange().second;

    // as our UVFs are always quantized to either 8bit or 16bit right now only the
    // nonfloat + unsigned path is taken, the others are for future extensions
    if (bIsFloat) {
      info.m_iValueType = 0;
    } else {
      if (bSigned) {
        info.m_iValueType = 1;
      } else {
        info.m_iValueType = 2;
      }
    }

    info.m_vAspect = Vec3f(v.GetScale());
    info.m_vDomainSize = v.GetDomainSize();
    info.m_iComponentSize = v.GetBitWidth();

    return true;
  } else {
    bool bAnalyzed = false;
    for (size_t i = 0;i<m_vpConverters.size();i++) {
      const vector<string>& vStrSupportedExt = m_vpConverters[i]->SupportedExt();
      for (size_t j = 0;j<vStrSupportedExt.size();j++) {
        if (vStrSupportedExt[j] == strExt) {
          bAnalyzed = m_vpConverters[i]->Analyze(strFilename, strTempDir, false, info);
          if (bAnalyzed) break;
        }
      }
      if (bAnalyzed) break;
    }

    if (!bAnalyzed && (m_pFinalConverter != 0)) {
      bAnalyzed = m_pFinalConverter->Analyze(strFilename, strTempDir, false, info);
    }

    return bAnalyzed;
  }
}

struct MergeableDatasets : public std::binary_function<Dataset, Dataset, bool> {
  bool operator()(const Dataset& a, const Dataset& b) const {
    if(a.GetComponentCount() != b.GetComponentCount() ||
       a.GetBrickOverlapSize() != b.GetBrickOverlapSize()) {
      return false;
    }

    const uint64_t timesteps = a.GetNumberOfTimesteps();
    if(timesteps != b.GetNumberOfTimesteps()) { return false; }

    const unsigned LoDs = a.GetLODLevelCount();
    if(LoDs != b.GetLODLevelCount()) { return false; }

    for(uint64_t ts=0; ts < timesteps; ++ts) {
      for(uint64_t level=0; level < LoDs; ++level) {
        const size_t st_ts = static_cast<size_t>(ts);
        const size_t st_level = static_cast<size_t>(level);
        if(a.GetDomainSize() != b.GetDomainSize() ||
           a.GetBrickCount(st_level, st_ts) != b.GetBrickCount(st_level, st_ts)) {
          return false;
        }
      }
    }

    return true;
  }
};

namespace {
  // interpolate a chunk of data into a new range.
  template<typename IForwIter, typename OForwIter, typename U>
  void interpolate(IForwIter ibeg, IForwIter iend,
                   const std::pair<double,double>& src_range,
                   OForwIter obeg)
  {
    const U max_out = std::numeric_limits<U>::max();
    assert(src_range.second >= src_range.first);
    const double diff = src_range.second - src_range.first;
    const double ifactor = max_out / diff;
    while(ibeg != iend) {
      *obeg = static_cast<U>((*ibeg - src_range.first) * ifactor);
      ++obeg;
      ++ibeg;
    }
  }
}

const std::shared_ptr<const RasterDataBlock> GetFirstRDB(const UVF& uvf)
{
  for(uint64_t i=0; i < uvf.GetDataBlockCount(); ++i) {
    if(uvf.GetDataBlock(i)->GetBlockSemantic() == UVFTables::BS_REG_NDIM_GRID)
    {
      return std::dynamic_pointer_cast<const RasterDataBlock>
                                      (uvf.GetDataBlock(i));
    }
  }
  return NULL;
}

namespace {
  template<typename T>
  std::pair<T,T> mm_init_dispatch(signed_tag) {
    return std::make_pair( std::numeric_limits<T>::max(),
                          -std::numeric_limits<T>::max());
  }
  template<typename T>
  std::pair<T,T> mm_init_dispatch(unsigned_tag) {
    return std::make_pair(std::numeric_limits<T>::min(),
                          std::numeric_limits<T>::max());
  }
  template<typename T>
  std::pair<T,T> mm_init() {
    typedef typename ctti<T>::sign_tag signedness;
    signedness s;
    return mm_init_dispatch<T>(s);
  }

  // a minmax algorithm that doesn't suck.  Namely, it takes an input iterator
  // instead of a forward iterator, *as it should*.  Jesus.
  // Also it returns 'T's, so you don't have to deref the return value.
  template<typename T, typename InputIterator>
  std::pair<T,T> minmax_input(InputIterator begin, InputIterator end) {
    std::pair<T,T> retval = mm_init<T>();
    while(begin != end) {
      retval.first  = std::min(*begin, retval.first);
      retval.second = std::max(*begin, retval.second);
      ++begin;
    }
    return retval;
  }
}

// converts 1D brick indices into RDB's indices.
std::vector<uint64_t> NDBrickIndex(const RasterDataBlock* rdb,
                                 size_t LoD, size_t b) {
  uint64_t brick = static_cast<uint64_t>(b);
  std::vector<uint64_t> lod(1);
  lod[0] = LoD;
  const std::vector<uint64_t>& counts = rdb->GetBrickCount(lod);

  uint64_t z = static_cast<uint64_t>(brick / (counts[0] * counts[1]));
  brick = brick % (counts[0] * counts[1]);
  uint64_t y = static_cast<uint64_t>(brick / counts[0]);
  brick = brick % counts[0];
  uint64_t x = brick;

  std::vector<uint64_t> vec(3);
  vec[0] = x;
  vec[1] = y;
  vec[2] = z;
  return vec;
}

namespace {
  /// Computes the minimum and maximum for a single brick in a raster data block.
  template<typename T>
  Vec4d get_brick_minmax(const RasterDataBlock* rdb,
                                 const std::vector<uint64_t>& vLOD,
                                 const std::vector<uint64_t>& vBrick)
  {
    Vec4d mmv;
    // min/max of gradients not supported...
    mmv.z = -std::numeric_limits<double>::max();
    mmv.w =  std::numeric_limits<double>::max();

    std::vector<T> data;
    rdb->GetData(data, vLOD, vBrick);
    std::pair<T,T> mm = minmax_input<T>(data.begin(), data.end());
    mmv.x = mm.first;
    mmv.y = mm.second;

    return mmv;
  }
}

/// Calculates the min/max scalar and gradient for every brick in a data set.
std::vector<Vec4d>
MaxMin(const RasterDataBlock* rdb)
{
  const bool is_signed = rdb->bSignedElement[0][0];
  const uint64_t bit_width = rdb->ulElementBitSize[0][0];
  const bool is_float = bit_width != rdb->ulElementMantissa[0][0];
  std::vector<Vec4d> mm;

  // We iterate over each LoD.  At each one, we iterate through the bricks.
  // When a GetData fails for that brick, we know we need to move on to the
  // next LoD.  When a GetData fails and we're at brick 0, we know we're done
  // with all of the LoDs.
  std::vector<uint64_t> vLOD(1);
  size_t brick;
  vLOD[0] = 0;
  do {
    brick = 0;
    size_t st_lod = static_cast<size_t>(vLOD[0]);
    do {
      std::vector<uint64_t> b_idx = NDBrickIndex(rdb, st_lod, brick);
      assert(rdb->ValidBrickIndex(vLOD, b_idx));
      /*
      IVDA_MESSAGE("%llu,%zu -> %llu,%llu,%llu", vLOD[0], brick,
              b_idx[0], b_idx[1], b_idx[2]);
      */

      if(is_float && bit_width == 32) {
        assert(is_signed);
        mm.push_back(get_brick_minmax<float>(rdb, vLOD, b_idx));
      } else if(is_float && bit_width == 64) {
        assert(is_signed);
        mm.push_back(get_brick_minmax<double>(rdb, vLOD, b_idx));
      } else if( is_signed &&  8 == bit_width) {
        mm.push_back(get_brick_minmax<int8_t>(rdb, vLOD, b_idx));
      } else if(!is_signed &&  8 == bit_width) {
        mm.push_back(get_brick_minmax<uint8_t>(rdb, vLOD, b_idx));
      } else if( is_signed && 16 == bit_width) {
        mm.push_back(get_brick_minmax<int16_t>(rdb, vLOD, b_idx));
      } else if(!is_signed && 16 == bit_width) {
        mm.push_back(get_brick_minmax<uint16_t>(rdb, vLOD, b_idx));
      } else if( is_signed && 32 == bit_width) {
        mm.push_back(get_brick_minmax<int32_t>(rdb, vLOD, b_idx));
      } else if(!is_signed && 32 == bit_width) {
        mm.push_back(get_brick_minmax<uint32_t>(rdb, vLOD, b_idx));
      } else if( is_signed && 64 == bit_width) {
        IVDA_ERROR("int64_t unsupported...");
        double mn = -std::numeric_limits<double>::max();
        double mx =  std::numeric_limits<double>::max();
        mm.push_back(Vec4d(mn,mx, mn,mx));
        assert(1 == 0);
      } else if(!is_signed && 64 == bit_width) {
        IVDA_ERROR("uint64_t unsupported...");
        double mn = -std::numeric_limits<double>::max();
        double mx =  std::numeric_limits<double>::max();
        mm.push_back(Vec4d(mn,mx, mn,mx));
        assert(1 == 0);
      } else {
        IVDA_ERROR("Unsupported data type!");
        assert(1 == 0);
      }
      IVDA_MESSAGEV("Finished lod,brick %u,%u", static_cast<unsigned>(vLOD[0]),
              static_cast<unsigned>(brick));
      ++brick;
    } while(rdb->ValidBrickIndex(vLOD, NDBrickIndex(rdb, st_lod, brick)));
    vLOD[0]++;
    st_lod = static_cast<size_t>(vLOD[0]);
  } while(rdb->ValidLOD(vLOD));
  return mm;
}

void
CreateUVFFromRDB(const std::string& filename,
                 const std::shared_ptr<const RasterDataBlock>& rdb)
{
  std::wstring wide_fn(filename.begin(), filename.end());
  UVF outuvf(wide_fn);
  outuvf.Create();

  GlobalHeader gh;
  gh.bIsBigEndian = EndianConvert::IsBigEndian();
  gh.ulChecksumSemanticsEntry = UVFTables::CS_MD5;
  outuvf.SetGlobalHeader(gh);

  outuvf.AddConstDataBlock(rdb);

  // create maxmin accel structures.  We'll need the maximum scalar
  // later, too, for computation of the 2D histogram.
  double max_val = DBL_MAX;
  {
    const size_t components = static_cast<size_t>(rdb->ulElementDimensionSize[0]);
    std::shared_ptr<MaxMinDataBlock> mmdb(new MaxMinDataBlock(components));
    std::vector<Vec4d> minmax = MaxMin(rdb.get());
    IVDA_MESSAGEV("found %u brick min/maxes...",
            static_cast<unsigned>(minmax.size()));
    for(std::vector<Vec4d>::const_iterator i = minmax.begin();
        i != minmax.end(); ++i) {
      // get the maximum maximum (that makes sense, I swear ;)
      max_val = std::max(max_val, i->y);

      // merge in the current brick's minmax.
      mmdb->StartNewValue();
      std::vector<Vec4d> tmp(1);
      tmp[0] = *i;
      mmdb->MergeData(tmp);
    }

    outuvf.AddDataBlock(mmdb);
  }

  { // histograms
    std::shared_ptr<Histogram1DDataBlock> hist1d(
      new Histogram1DDataBlock()
    );
    hist1d->Compute(rdb.get());
    outuvf.AddDataBlock(hist1d);
    {
      std::shared_ptr<Histogram2DDataBlock> hist2d(
        new Histogram2DDataBlock()
      );
      hist2d->Compute(rdb.get(), hist1d->GetHistogram().size(), max_val);
      outuvf.AddDataBlock(hist2d);
    }
  }

  outuvf.Close();
}


// Identifies the 'widest' type that is utilized in a series of UVFs.
// For example, if we've got FP data in one UVF and unsigned bytes in
// another, the 'widest' type is FP.
void IdentifyType(const std::vector<std::shared_ptr<UVFDataset>>& uvf,
                  size_t& bit_width, bool& is_float, bool &is_signed)
{
  bit_width = 0;
  is_float = false;
  is_signed = false;

  for(size_t i=0; i < uvf.size(); ++i) {
    bit_width = std::max(bit_width, static_cast<size_t>(uvf[i]->GetBitWidth()));
    is_float = std::max(is_float, uvf[i]->GetIsFloat());
    is_signed = std::max(is_signed, uvf[i]->GetIsSigned());
  }
}

// Reads in data of the given type.  If data is not stored that way in
// the file, it will expand it out to the given type.  Assumes it will
// always be expanding data, never compressing it!
template<typename T>
void TypedRead(std::vector<T>& data,
               const Dataset& ds,
               const BrickKey& key)
{
  size_t width = static_cast<size_t>(ds.GetBitWidth());
  bool is_signed = ds.GetIsSigned();
  bool is_float = ds.GetIsFloat();

  size_t dest_width = sizeof(T) * 8;
  bool dest_signed = ctti<T>::is_signed;
  bool dest_float = std::is_floating_point<T>::value;

  // fp data implies signed data.
  assert(is_float ? is_signed : true);
  assert(dest_float ? dest_signed : true);

  IVDA_MESSAGEV(" [Source Data] Signed: %d  Float: %d  Width: %u",
          is_signed, is_float, static_cast<unsigned>(width));
  IVDA_MESSAGEV(" [Destination] Signed: %d  Float: %d  Width: %u",
          dest_signed, dest_float, static_cast<unsigned>(dest_width));

  // If we're lucky, we can just read the data and be done with it.
  if(dest_width == width && dest_signed == is_signed &&
     dest_float == is_float) {
    IVDA_MESSAGE("Data is stored the way we need it!  Yay.");
    ds.GetBrick(key, data);
    return;
  }

  // Otherwise we'll need to read it into a temporary buffer and expand
  // it into the argument vector.
  std::pair<double, double> range = ds.GetRange();

  if(is_float && width == 32) {
    std::vector<float> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<float*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if(is_float && width == 64) {
    // Can this happen?  What would we expand double into?
    std::vector<double> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<double*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if( is_signed &&  8 == width) {
    std::vector<int8_t> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<int8_t*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if(!is_signed &&  8 == width) {
    std::vector<uint8_t> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<uint8_t*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if( is_signed && 16 == width) {
    std::vector<int16_t> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<int16_t*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if(!is_signed && 16 == width) {
    std::vector<uint16_t> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<uint16_t*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if( is_signed && 32 == width) {
    std::vector<int32_t> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<int32_t*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else if(!is_signed && 32 == width) {
    std::vector<uint32_t> tmpdata;
    ds.GetBrick(key, tmpdata);
    data.resize(tmpdata.size() / (width/8));
    interpolate<uint32_t*, typename std::vector<T>::iterator, T>(
      &tmpdata[0], (&tmpdata[0]) + tmpdata.size(), range, data.begin()
    );
  } else {
    IVDA_ERRORV("Unhandled data type!  Width: %u, Signed: %d, Float: %d",
            static_cast<unsigned>(width), is_signed, is_float);
  }
}

#ifdef COMPILE_EXPRESSIONS
namespace {
  template<typename T>
  void ReadAndEvalBrick(
    RasterDataBlock& rdb,
    const std::vector<std::shared_ptr<UVFDataset>>& uvfs,
    const std::vector<BrickTable::const_iterator>& iters,
    Tuvok::expression::Node* tree
  ) {
    std::vector<std::vector<T>> involumes(uvfs.size());
    std::vector<T> output;
    for(size_t i=0; i < uvfs.size(); ++i) {
      IVDA_MESSAGEV("Reading brick from volume %u/%u...", static_cast<unsigned>(i+1),
              static_cast<unsigned>(uvfs.size()));
      TypedRead<T>(involumes[i], *uvfs[i], iters[i]->first);
    }
    IVDA_MESSAGE("Evaluating expression ...");
    Tuvok::expression::evaluate(*tree, involumes, output);

    IVDA_MESSAGE("Writing ...");
    NDBrickKey nk = uvfs[0]->IndexToVectorKey(iters[0]->first);
    if(false == rdb.SetData(&output[0], nk.lod, nk.brick)) {
      IVDA_ERROR("Write failed!");
    }
  }
}

struct cleanup_parser {
  ~cleanup_parser() { parser_free(); }
};

void
IOManager::EvaluateExpression(const std::string& expr,
                              const std::vector<std::string>& volumes,
                              const std::string& out_fn) const
                              throw(Tuvok::Exception)
{
  parser_set_string(expr.c_str());
  int parse_err = yyparse();
  std::unique_ptr<cleanup_parser> p(new cleanup_parser());
  assert(!volumes.empty());

  if(parse_err == 1) {
    throw Tuvok::expression::SyntaxError("", 0, 2, __FILE__, __LINE__);
  }

  // open all of those files and get UVF datasets for each of them.
  const bool verify=false;
  std::vector<std::shared_ptr<UVFDataset>> uvf;
  typedef std::vector<std::string>::const_iterator citer;
  for(citer f = volumes.begin(); f != volumes.end(); ++f) {
    uvf.push_back(std::shared_ptr<UVFDataset>(
      dynamic_cast<UVFDataset*>(
        m_dsFactory->Create(*f, 256 /* hack! */, verify)
      )
    ));
  }
  // ensure those UVFs are "equal" in some sense (same number of voxels, etc).
  MergeableDatasets mergeable;
  typedef std::vector<std::shared_ptr<UVFDataset>>::const_iterator uiter;
  for(uiter u = uvf.begin(); u != uvf.end(); ++u) {
    if(!mergeable(**uvf.begin(), **u)) {
      throw Tuvok::io::UnmergeableDatasets("Incompatible input volumes",
                                           __FILE__, __LINE__);
    }
  }

  Tuvok::expression::Node* tree = parser_tree_root();

  // volume iterators
  std::vector<BrickTable::const_iterator> viters;
#ifdef DETECTED_OS_APPLE
  viters.reserve(uvf.size());
  for(size_t i=0; i < uvf.size(); ++i) {
    viters.push_back(uvf[i]->BricksBegin());
  }
#else
  viters.resize(uvf.size());
  for(size_t i=0; i < viters.size(); ++i) { // initialize
    viters[i] = uvf[i]->BricksBegin();
  }
#endif

  std::shared_ptr<RasterDataBlock> rdb(new RasterDataBlock());
  rdb->SetBlockSemantic(UVFTables::BS_REG_NDIM_GRID);
  rdb->SetIdentityTransformation();
  rdb->SetTypeToUShort(UVFTables::ES_RED);

  { // Copy the other basic info from the first input volume.
    std::wstring wide_vol(volumes[0].begin(), volumes[0].end());
    UVF firstvol(wide_vol);
    firstvol.Open(false, false, false);
    const std::shared_ptr<const RasterDataBlock> rdb1 = GetFirstRDB(firstvol);
    // Received this exception when using datasets that were converted to the
    // octree based UVF format (happens with experimental features enabled in
    // settings -- see RAWConverter::ConvertRAWDataset(...) ).
    if (rdb1 == NULL) {
      throw Tuvok::io::IOException("No raster data blocks present in the "
                                   "first volume.",
                                   __FILE__, __LINE__);
    }
    *rdb = *rdb1;
  }

  std::string tmp_fn = RemoveExt(out_fn) + ".rdb";
  LargeRAWFile_ptr lout(new TempFile(tmp_fn));
  lout->Create();
  rdb->ResetFile(lout);

  // Figure out which what type our output data should be.
  size_t bit_width;
  bool is_float, is_signed;
  IdentifyType(uvf, bit_width, is_float, is_signed);

  //     foreach brick:
  //       load brick into 'involumes'
  //       evaluate(tree, input-bricks-in-a-vector, output)
  //       write output somewhere
  /// @todo FIXME: we should query bit_width, is_float, is_signed to create
  /// different 'involumes' based on the type we need...
  size_t brick = 0;
  while(viters[0] != uvf[0]->BricksEnd()) {
    for(size_t i=0; i < uvf.size(); ++i) {
      IVDA_MESSAGEV("Brick %u (file %03u/%03u)...", static_cast<unsigned>(brick),
              static_cast<unsigned>(i+1), static_cast<unsigned>(uvf.size()));
      // Read in the data we need.
      if(is_float && bit_width == 32) {
        ReadAndEvalBrick<float>(*rdb, uvf, viters, tree);
      } else if(is_float && bit_width == 64) {
        // Not implemented in UVF...
        IVDA_ERROR("double format data not supported!");
        continue;
      } else if( is_signed && bit_width ==  8) {
        ReadAndEvalBrick< int8_t>(*rdb, uvf, viters, tree);
      } else if(!is_signed && bit_width ==  8) {
        ReadAndEvalBrick<uint8_t>(*rdb, uvf, viters, tree);
      } else if( is_signed && bit_width == 16) {
        ReadAndEvalBrick< int16_t>(*rdb, uvf, viters, tree);
      } else if(!is_signed && bit_width == 16) {
        ReadAndEvalBrick<uint16_t>(*rdb, uvf, viters, tree);
      // These types aren't yet implemented in UVF/RasterDataBlock.
      } else if( is_signed && bit_width == 32) {
        IVDA_ERROR("32bit signed int data not implemented!");
        //ReadAndEvalBrick< int32_t>(*rdb, uvf, viters, tree);
      } else if(!is_signed && bit_width == 32) {
        IVDA_ERROR("32bit unsigned data not implemented!");
        //ReadAndEvalBrick<uint32_t>(*rdb, uvf, viters, tree);
      } else if( is_signed && bit_width == 64) {
        IVDA_ERROR("64bit signed int data not implemented!");
        //ReadAndEvalBrick< int64_t>(*rdb, uvf, viters, tree);
      } else if(!is_signed && bit_width == 64) {
        IVDA_ERROR("64bit unsigned data not implemented!");
        //ReadAndEvalBrick<uint64_t>(*rdb, uvf, viters, tree);
      } else {
        IVDA_ERROR("Could not figure out destination data type!");
      }
    }
    IVDA_MESSAGEV("Brick %u (evaluation)...", static_cast<unsigned>(brick));

    using namespace std::placeholders;
    // advance each brick iterator by one.
#ifndef __APPLE__
    std::for_each(viters.begin(), viters.end(),
                  std::bind(std::advance<BrickTable::const_iterator,int>,
                            _1, 1));
#else
    // Clang's libc++ uses iterator_traits<_InputIter>::difference_type to
    // deduce distance template parameter.
    std::for_each(viters.begin(), viters.end(),
                  std::bind(std::advance<BrickTable::const_iterator>,
                            _1, 1));
#endif
  }

  CreateUVFFromRDB(out_fn, rdb);
}
#endif

bool IOManager::ReBrickDataset(const string& strSourceFilename,
                               const string& strTargetFilename,
                               const string& strTempDir,
                               const uint64_t iMaxBrickSize,
                               const uint64_t iBrickOverlap,
                               bool bQuantizeTo8Bit) const {
  IVDA_MESSAGE("Rebricking (Phase 1/2)...");

  string filenameOnly = GetFilename(strSourceFilename);
  string tmpFile = strTempDir+ChangeExt(filenameOnly,"nrrd"); /// use some simple format as intermediate file

  if (!ConvertDataset(strSourceFilename, tmpFile, strTempDir, false,
                      m_iBuilderBrickSize, m_iBrickOverlap, false)) {
    IVDA_ERRORV("Unable to extract raw data from file %s to %s", strSourceFilename.c_str(),tmpFile.c_str());
    return false;
  }

  IVDA_MESSAGE("Rebricking (Phase 2/2)...");

  if (!ConvertDataset(tmpFile, strTargetFilename, strTempDir, true,
                      iMaxBrickSize, iBrickOverlap,bQuantizeTo8Bit)) {
    IVDA_ERRORV("Unable to convert raw data from file %s into new UVF file %s", tmpFile.c_str(),strTargetFilename.c_str());
    if(remove(tmpFile.c_str()) == -1)IVDA_WARNINGV("Unable to delete temp file %s", tmpFile.c_str());
    return false;
  }
  if(remove(tmpFile.c_str()) == -1)IVDA_WARNINGV("Unable to delete temp file %s", tmpFile.c_str());

  return true;
}


void IOManager::CopyToTSB(const Mesh& m, GeometryDataBlock* tsb) const {
  // source data
  const VertVec&      v = m.GetVertices();
  const NormVec&      n = m.GetNormals();
  const TexCoordVec&  t = m.GetTexCoords();
  const ColorVec&     c = m.GetColors();

  // target data
  vector<float> fVec;
  size_t iVerticesPerPoly = m.GetVerticesPerPoly();
  tsb->SetPolySize(iVerticesPerPoly );

  if (!v.empty()) {fVec.resize(v.size()*3); memcpy(&fVec[0],&v[0],v.size()*3*sizeof(float)); tsb->SetVertices(fVec);}
  if (!n.empty()) {fVec.resize(n.size()*3); memcpy(&fVec[0],&n[0],n.size()*3*sizeof(float)); tsb->SetNormals(fVec);}
  if (!t.empty()) {fVec.resize(t.size()*2); memcpy(&fVec[0],&t[0],t.size()*2*sizeof(float)); tsb->SetTexCoords(fVec);}
  if (!c.empty()) {fVec.resize(c.size()*4); memcpy(&fVec[0],&c[0],c.size()*4*sizeof(float)); tsb->SetColors(fVec);}

  tsb->SetVertexIndices(m.GetVertexIndices());
  tsb->SetNormalIndices(m.GetNormalIndices());
  tsb->SetTexCoordIndices(m.GetTexCoordIndices());
  tsb->SetColorIndices(m.GetColorIndices());

  tsb->m_Desc = m.Name();
}


std::shared_ptr<Mesh> IOManager::LoadMesh(const string& meshfile) const
{
  IVDA_MESSAGE("Opening Mesh File ...");

  // iterate through all our converters, stopping when one successfully
  // converts our data.
  std::shared_ptr<Mesh> m;
  for(vector<AbstrGeoConverter*>::const_iterator conv =
      m_vpGeoConverters.begin(); conv != m_vpGeoConverters.end(); ++conv) {
    IVDA_MESSAGEV("Attempting converter '%s'", (*conv)->GetDesc().c_str());
    if((*conv)->CanRead(meshfile)) {
      IVDA_MESSAGEV("Converter '%s' can read '%s'!",
              (*conv)->GetDesc().c_str(), meshfile.c_str());
      try {
        m = (*conv)->ConvertToMesh(meshfile);
      } catch (const std::exception& err) {
       IVDA_WARNINGV("Converter %s can read files, but conversion failed: %s",
                (*conv)->GetDesc().c_str(), err.what());
        throw;
      }
      break;
    }
  }
  return m;
}

void IOManager::AddMesh(const UVF* sourceDataset,
                        const string& meshfile,
                        const string& uvf_fn) const
{
  std::shared_ptr<Mesh> m = LoadMesh(meshfile);

  if (!m) {
   IVDA_WARNINGV("No converter for geometry file %s can be found",
            meshfile.c_str());
    throw Tuvok::io::DSOpenFailed(meshfile.c_str(), __FILE__, __LINE__);
  }

  // make sure we have at least normals
  if (m->GetNormalIndices().empty()) m->RecomputeNormals();

  // now create a GeometryDataBlock ...
  std::shared_ptr<GeometryDataBlock> tsb(new GeometryDataBlock());

  // ... and transfer the data from the mesh object
  CopyToTSB(*m, tsb.get());

  wstring wuvf(uvf_fn.begin(), uvf_fn.end());
  UVF uvfFile(wuvf);
  GlobalHeader uvfGlobalHeader;
  uvfGlobalHeader.bIsBigEndian = EndianConvert::IsBigEndian();
  uvfGlobalHeader.ulChecksumSemanticsEntry = UVFTables::CS_MD5;
  uvfFile.SetGlobalHeader(uvfGlobalHeader);

  for(uint64_t i = 0; i<sourceDataset->GetDataBlockCount(); i++) {
    uvfFile.AddConstDataBlock(sourceDataset->GetDataBlock(i));
  }

  IVDA_MESSAGE("Adding triangle soup block...");
  uvfFile.AddDataBlock(tsb);

  uvfFile.Create();
  IVDA_MESSAGE("Computing checksum...");
  uvfFile.Close();
}

bool IOManager::SetMaxBrickSize(uint64_t iMaxBrickSize,
                                uint64_t iBuilderBrickSize) {
  if (iMaxBrickSize > m_iBrickOverlap && iBuilderBrickSize > m_iBrickOverlap) {
    m_iMaxBrickSize = iMaxBrickSize;
    m_iBuilderBrickSize = iBuilderBrickSize;
    return true;
  } else return false;
}

bool IOManager::SetBrickOverlap(const uint64_t iBrickOverlap) {
  if (m_iMaxBrickSize > iBrickOverlap && m_iBuilderBrickSize > m_iBrickOverlap) {
    m_iBrickOverlap = iBrickOverlap;
    return true;
  } else return false;
}
