/****************************************************************
 * Copyright (C) 2016 Alexander Schiewe
 *
 * The content of this file may not be copied and/or distributed
 * without the expressed permission of the copyright owner.
 *
 ****************************************************************/

#pragma once

#include <cstdint>
#include <vector>

namespace trinity {
  
class JPEGDecoder
{
public:
  enum PixelFormat {
    Format_RGB = 0,
    Format_RGBA = 7
  };

  /**
   * @param format is the pixel format of the decoded raw buffer
   * @param fastUpsampling can be used when decompressing an image that was compressed using
   * chrominance subsampling, in order to use the fastest chrominance upsampling algorithm
   * available in the underlying codec. The default is to use smooth upsampling but potentially slower
   * decompression, which creates a smooth transition between neighboring chrominance components in
   * order to reduce upsampling artifacts in the decompressed image.
  */
  JPEGDecoder(PixelFormat format = Format_RGBA, bool fastUpsampling = false);
  JPEGDecoder(JPEGDecoder const&) = delete;
  ~JPEGDecoder();

  JPEGDecoder& operator=(JPEGDecoder const&) = delete;
  
  void setFormat(PixelFormat format);
  PixelFormat getFormat() const;
  void setFastUpsampling(bool fastUpsampling);
  bool getFastUpsampling() const;
  
  std::vector<uint8_t> decode(const std::vector<uint8_t>& jpeg) const;
  std::vector<uint8_t> decode(const std::vector<uint8_t>& jpeg, int& width, int& height) const;

private:
  void * m_handle;
  PixelFormat m_format;
  int m_flags;
};
  
}
