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
  \file    TiffVolumeConverter.h
  \author  Tom Fogal
           SCI Institute
           University of Utah
*/
#include <cstring>
#ifndef TUVOK_NO_IO
# include <libtiff/tiffio.h>
#else
  struct TIFF;
#endif
#include "TiffVolumeConverter.h"
#include <Core/StdTuvokDefines.h>
#include <Core/IO/FileTools.h>
using namespace Core::IO::FileTools;

#ifdef __GNUC__
# define _malloc __attribute__((malloc))
#else
# define _malloc /* nothing */
#endif

static void tv_dimensions(TIFF *, size_t dims[3]);
_malloc static uint8_t* tv_read_slice(TIFF *);

TiffVolumeConverter::TiffVolumeConverter()
{
  m_vConverterDesc = "TIFF Volume (Image stack)";
#ifndef TUVOK_NO_IO
  m_vSupportedExt.push_back("OME.TIF");
  m_vSupportedExt.push_back("OME.TIFF");
  m_vSupportedExt.push_back("TIF");
  m_vSupportedExt.push_back("TIFF");
#endif
}

// converts a TiffVolume to a `raw' file.  We'll read through the TIFF
// slice-by-slice, copying each slice to the raw file.
bool
TiffVolumeConverter::ConvertToRAW(const std::string& strSourceFilename,
                                  const std::string& strTempDir,
                                  bool, uint64_t& iHeaderSkip,
                                  unsigned& iComponentSize,
                                  uint64_t& iComponentCount,
                                  bool& bConvertEndianess, bool& bSigned,
                                  bool& bIsFloat, Vec3ui64& vVolumeSize,
                                  Vec3f& vVolumeAspect,
                                  std::string& strTitle,
                                  std::string& strIntermediateFile,
                                  bool& bDeleteIntermediateFile)
{
#ifndef TUVOK_NO_IO
 IVDA_MESSAGEV("Attempting to convert TiffVolume: %s", strSourceFilename.c_str());

  TIFF *tif = TIFFOpen(strSourceFilename.c_str(), "r");
  if(tif == NULL) {
    IVDA_ERRORV("Could not open %s", strSourceFilename.c_str());
    return false;
  }

  // Get the dimensions of the volume.
  {
    size_t dims[3];
    tv_dimensions(tif, dims);
    vVolumeSize[0] = static_cast<uint64_t>(dims[0]);
    vVolumeSize[1] = static_cast<uint64_t>(dims[1]);
    vVolumeSize[2] = static_cast<uint64_t>(dims[2]);
   IVDA_MESSAGEV("TiffVolume dimensions: %ux%ux%u", static_cast<unsigned>(dims[0]),
                                               static_cast<unsigned>(dims[1]),
                                               static_cast<unsigned>(dims[2]));
    if(dims[2] <= 1) {
      IVDA_ERROR("TIFF is not a volume; use "
              "`Load Dataset from Directory' instead!");
      TIFFClose(tif);
      return false;
    }
  }
  iHeaderSkip = 0;

  // read the number of bits per component from the tiff tag.
  {
    uint16_t bits_per_sample;
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    iComponentSize = bits_per_sample;
   IVDA_MESSAGEV("%ld bits per component.", iComponentSize);
  }
  // likewise for the number of components / pixel.
  {
    uint16_t components;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &components);
    iComponentCount = components;
    {
      std::ostringstream com;
      com << iComponentCount << " component";
      if(components > 1) { com << "s"; }
      com << ".";
     IVDA_MESSAGEV("%s", com.str().c_str());
    }
  }
  // Libtiff handles the endian issue for us.
  bConvertEndianess = false;

  // Data type/kind: signed or unsigned, floating point or not.
  {
    bSigned = false;
    bIsFloat = false;
    uint16_t sf;
    if(TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sf) != 0) {
      bSigned = (sf == SAMPLEFORMAT_INT) || (sf == SAMPLEFORMAT_IEEEFP) ||
                (sf == SAMPLEFORMAT_COMPLEXINT);
      bIsFloat = (sf == SAMPLEFORMAT_IEEEFP);
    }
  }
  // Not sure if these are readable/stored in a TIFF.
  vVolumeAspect[0] = 1;
  vVolumeAspect[1] = 1;
  vVolumeAspect[2] = 1;

  strTitle = "TIFF Volume";

  // Create an intermediate file to hold the data.
  strIntermediateFile = strTempDir +
                        GetFilename(strSourceFilename) + ".binary";
  LargeRAWFile binary(strIntermediateFile);
  binary.Create(iComponentSize/8 * iComponentCount * vVolumeSize.volume());
  if(!binary.IsOpen()) {
    IVDA_ERRORV("Could not create binary file %s", strIntermediateFile.c_str());

    TIFFClose(tif);
    return false;
  }
  bDeleteIntermediateFile = true;
  // Populate the intermediate file.  We'll do this slice-by-slice, which isn't
  // exactly kosher for this library -- a slice could technically be larger
  // than INCORESIZE.  But it won't be.
  unsigned slice_idx=0;
  do {
   IVDA_MESSAGEV("Reading %llux%llu TIFF slice %u of %llu",
            vVolumeSize[0], vVolumeSize[1], slice_idx++, vVolumeSize[2]-1);
    uint8_t* slice = tv_read_slice(tif);
    if(slice) {
      // assuming 8-bit data here, which might not always be valid.
      binary.WriteRAW(static_cast<unsigned char*>(slice),
                      vVolumeSize[0]*vVolumeSize[1] *
                      sizeof(uint8_t)*iComponentCount);
      _TIFFfree(slice);
    } else {
      binary.Close();
      binary.Delete();
      TIFFClose(tif);
      return false;
    }
  } while(TIFFReadDirectory(tif));
  binary.Close();

  TIFFClose(tif);
  return true;
#else
  IVDA_ERROR("Tuvok was not built with IO support!");
  return false;
#endif
}

// unimplemented!
bool
TiffVolumeConverter::ConvertToNative(const std::string&,
                                     const std::string&,
                                     uint64_t, unsigned,
                                     uint64_t, bool,
                                     bool,
                                     Vec3ui64,
                                     Vec3f,
                                     bool,
                                     bool)
{
  return false;
}

// Reads the dimensions of the TIFF volume.  X and Y come from the dimensions
// of the first image in the stack: we assume that this stays constant
// throughout the volume.  Z comes from the number of images in the stack.
static void
tv_dimensions(TIFF *tif, size_t dims[3])
{
#ifndef TUVOK_NO_IO
  uint32_t x,y;
  size_t z=0;

  TIFFSetDirectory(tif, 0);
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &x);
  // tiff calls the height "length" for some reason.
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &y);
  do {
    uint32_t tmpx, tmpy;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &tmpx);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &tmpy);
    if(tmpx != x) {IVDA_WARNING("TIFF x dimension changes in stack!"); }
    if(tmpy != y) {IVDA_WARNING("TIFF y dimension changes in stack!"); }
    ++z;
  } while(TIFFReadDirectory(tif));
  TIFFSetDirectory(tif, 0);

  dims[0] = x;
  dims[1] = y;
  dims[2] = z;
#endif
}

_malloc static uint8_t*
tv_read_slice(TIFF *tif)
{
#ifdef TUVOK_NO_IO
  return NULL;
#else
  uint8_t *slice;
  uint32_t width;
  uint32_t height;
  uint16_t bpp;
  uint16_t components;
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bpp);
  TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &components);

  if(bpp != 8) {
    IVDA_ERRORV("%hu-bit data is unsupported!", bpp);
    // Actually, this routine should handle it fine,
    // it's just that the caller and probably the rest
    // of Tuvok doesn't handle this well.
    return NULL;
  }

  const tsize_t slice_sz = width*height*(bpp/8)*components * sizeof(uint8_t);
  slice = static_cast<uint8_t*>(_TIFFmalloc(slice_sz));
  if(slice == NULL) {
    IVDA_ERROR("TIFFmalloc failed.");
    return NULL;
  }

  const tstrip_t n_strips = TIFFNumberOfStrips(tif);
  {
    uint8_t *data = slice;
    tdata_t buf = static_cast<tdata_t>(_TIFFmalloc(TIFFStripSize(tif)));
    // strips shouldn't be bigger than slices.
    assert(TIFFStripSize(tif) <= slice_sz);
    for(tstrip_t s=0; s < n_strips; ++s) {
      tsize_t n_bytes;
      n_bytes = TIFFReadEncodedStrip(tif, s, buf, static_cast<tsize_t>(-1));
      std::memcpy(data, buf, n_bytes);
      data += n_bytes;
    }
    _TIFFfree(buf);
  }

  return slice;
#endif
}
