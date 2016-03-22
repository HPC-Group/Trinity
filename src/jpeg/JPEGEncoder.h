/****************************************************************
 * Copyright (C) 2016 Alexander Schiewe
 *
 * The content of this file may not be copied and/or distributed
 * without the expressed permission of the copyright owner.
 *
 ****************************************************************/

#pragma once

#include "mocca/base/Nullable.h"
#include "mocca/base/ByteArray.h"

namespace trinity {

class JPEGEncoder
{
public:
  enum ChrominanceSubsampling {
    Subsample_444 = 0,
    Subsample_422,
    Subsample_420,
    Subsample_Gray,
    Subsample_440,
    Subsample_411
  };

  /**
   * @param quality is the image quality of the generated JPEG image (1 = worst, 100 = best)
   * https://github.com/libjpeg-turbo/libjpeg-turbo#fast-integer-forward-dct-at-high-quality-levels
   * The algorithm used by the SIMD-accelerated quantization function cannot produce correct results
   * whenever the fast integer forward DCT is used along with a JPEG quality of 98-100. Thus,
   * libjpeg-turbo must use the non-SIMD quantization function in those cases. This causes
   * performance to drop by as much as 40%. It is therefore strongly advised that you use the slow
   * integer forward DCT whenever encoding images with a JPEG quality of 98 or higher.
   * @param subsampling method, 4:4:0 and 4:1:1 are not fully accelerated yet by libjpeg-turbo
  */
  JPEGEncoder(int quality = 75, ChrominanceSubsampling subsampling = Subsample_444);
  JPEGEncoder(JPEGEncoder const&) = delete;
  ~JPEGEncoder();
  
  JPEGEncoder& operator=(JPEGEncoder const&) = delete;
  
  void setQuality(int quality);
  int getQuality() const;
  void setSubsampling(ChrominanceSubsampling subsampling);
  ChrominanceSubsampling getSubsampling() const;
  
  mocca::Nullable<mocca::ByteArray> encode(mocca::ByteArray const& raw, int width, int height);
  
private:
  void * m_handle;
  int m_quality;
  ChrominanceSubsampling m_subsampling;
  unsigned char * m_buffer;
  unsigned long m_size;
};

}
