/****************************************************************
 * Copyright (C) 2016 Alexander Schiewe
 *
 * The content of this file may not be copied and/or distributed
 * without the expressed permission of the copyright owner.
 *
 ****************************************************************/

#include "JPEGEncoder.h"
#include <turbojpeg.h>
#include "mocca/log/LogManager.h"

using namespace trinity;
using namespace mocca;

static_assert(TJSAMP_444 == static_cast<TJSAMP>(JPEGEncoder::Subsample_444), "enum mismatch");
static_assert(TJSAMP_422 == static_cast<TJSAMP>(JPEGEncoder::Subsample_422), "enum mismatch");
static_assert(TJSAMP_420 == static_cast<TJSAMP>(JPEGEncoder::Subsample_420), "enum mismatch");
static_assert(TJSAMP_GRAY == static_cast<TJSAMP>(JPEGEncoder::Subsample_Gray), "enum mismatch");
static_assert(TJSAMP_440 == static_cast<TJSAMP>(JPEGEncoder::Subsample_440), "enum mismatch");
static_assert(TJSAMP_411 == static_cast<TJSAMP>(JPEGEncoder::Subsample_411), "enum mismatch");

JPEGEncoder::JPEGEncoder(int quality, ChrominanceSubsampling subsampling)
: m_handle(tjInitCompress())
, m_quality(quality)
, m_subsampling(subsampling)
, m_buffer(nullptr)
, m_size(0)
{
  if (m_handle == nullptr) {
    LERROR("turbojpeg: " << tjGetErrorStr());
    // TODO: graceful exit, throw some trinity exception?
  }
}

JPEGEncoder::~JPEGEncoder()
{
  tjFree(m_buffer);
  
  if (tjDestroy(m_handle) != 0) {
    LERROR("turbojpeg: " << tjGetErrorStr());
  }
}

void JPEGEncoder::setQuality(int quality)
{
  m_quality = quality;
}

int JPEGEncoder::getQuality() const
{
  return m_quality;
}

void JPEGEncoder::setSubsampling(ChrominanceSubsampling subsampling)
{
  m_subsampling = subsampling;
}

JPEGEncoder::ChrominanceSubsampling JPEGEncoder::getSubsampling() const
{
  return m_subsampling;
}

Nullable<ByteArray> JPEGEncoder::encode(ByteArray const& raw, int width, int height)
{
  int channels = raw.size() / (width * height);
//  LINFO("encoding: " << width << "x" << height << "@" << channels << " channels with quality " << m_quality << " and subsampling " << m_subsampling);

  // TODO: introduce and use some reasonable pixel format description within trinity
  int format;
  switch (channels) {
    case 3:
      format = TJPF_RGB;
      break;
    case 4:
      format = TJPF_RGBA;
      break;
    default:
      LERROR("unsupported pixel format");
      return Nullable<ByteArray>();
  }
  
  // libjpeg-turbo 1.4.2 doc promises to not modify the src buffer but does not decalre it as const
  // libjpeg-turbo 1.5 which is still in beta fixes this issue
  unsigned char * src = const_cast<unsigned char *>(raw.data());
  int pitch = width * tjPixelSize[format];
  int flags = TJFLAG_BOTTOMUP;

  int success = tjCompress2(m_handle, src, width, pitch, height, format, &m_buffer, &m_size, m_subsampling, m_quality, flags);
  
  if (success != 0) {
    LERROR("turbojpeg: " << tjGetErrorStr());
    return Nullable<ByteArray>();
  }

  ByteArray jpeg = ByteArray::createFromRaw(m_buffer, m_size);
  
//  LINFO("raw: " << raw.size() << " bytes, jpeg: " << jpeg.size() << " bytes, compression ratio: " << (float)raw.size()/jpeg.size());

  return std::move(jpeg);
}
