/****************************************************************
 * Copyright (C) 2016 Alexander Schiewe
 *
 * The content of this file may not be copied and/or distributed
 * without the expressed permission of the copyright owner.
 *
 ****************************************************************/

#include <turbojpeg.h>

#include "JPEGDecoder.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Memory.h"

using namespace trinity;
using namespace mocca;

static_assert(TJPF_RGB == static_cast<TJPF>(JPEGDecoder::Format_RGB), "enum mismatch");
static_assert(TJPF_RGBA == static_cast<TJPF>(JPEGDecoder::Format_RGBA), "enum mismatch");

JPEGDecoder::JPEGDecoder(PixelFormat format, bool fastUpsampling)
    : m_handle(tjInitDecompress())
    , m_format(format)
    , m_flags(TJFLAG_BOTTOMUP) {
    if (m_handle == nullptr) {
        LERROR("turbojpeg: " << tjGetErrorStr());
        // TODO: graceful exit, throw some trinity exception?
    }
    setFastUpsampling(fastUpsampling);
}

JPEGDecoder::~JPEGDecoder() {
    if (tjDestroy(m_handle) != 0) {
        LERROR("turbojpeg: " << tjGetErrorStr());
    }
}

void JPEGDecoder::setFormat(PixelFormat format) {
    m_format = format;
}

JPEGDecoder::PixelFormat JPEGDecoder::getFormat() const {
    return m_format;
}

void JPEGDecoder::setFastUpsampling(bool fastUpsampling) {
    if (fastUpsampling)
        m_flags |= TJFLAG_FASTUPSAMPLE;
    else
        m_flags &= ~TJFLAG_FASTUPSAMPLE;
}

bool JPEGDecoder::getFastUpsampling() const {
    return m_flags & TJFLAG_FASTUPSAMPLE;
}

std::unique_ptr<std::vector<uint8_t>> JPEGDecoder::decode(const std::vector<uint8_t>& jpeg) const {
    int width, height;
    return decode(jpeg, width, height);
}

std::unique_ptr<std::vector<uint8_t>> JPEGDecoder::decode(const std::vector<uint8_t>& jpeg, int& width, int& height) const {
    // libjpeg-turbo 1.4.2 doc promises to not modify the src buffer but does not decalre it as const
    // libjpeg-turbo 1.5 which is still in beta fixes this issue
    unsigned char* src = const_cast<unsigned char*>(jpeg.data());

    int subsampling, colorspace;
    int success = tjDecompressHeader3(m_handle, src, jpeg.size(), &width, &height, &subsampling, &colorspace);

    if (success != 0) {
        LERROR("turbojpeg: " << tjGetErrorStr());
        return nullptr;
    }

    //  LINFO("decoding: " << width << "x" << height << " subsampling " << subsampling << " colorspace " << colorspace);

    int pitch = width * tjPixelSize[m_format];
    size_t size = pitch * height;
    auto raw = mocca::make_unique<std::vector<uint8_t>>(size);

    success = tjDecompress2(m_handle, src, jpeg.size(), raw->data(), width, pitch, height, m_format, m_flags);

    if (success != 0) {
        LERROR("turbojpeg: " << tjGetErrorStr());
        return nullptr;
    }

    //  LINFO("raw: " << raw.size() << " bytes, jpeg: " << jpeg.size() << " bytes, compression ratio: " << (float)raw.size()/jpeg.size());

    return raw;
}
