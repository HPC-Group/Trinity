#include <sstream>

#include "QVisVolume.h"
#include "QVisVolumeMetadata.h"
#include "VolumeError.h"
#include <core/StringTools.h>

namespace DataIO {
  namespace VolumeIO {

    QVisVolume::QVisVolume(const std::string& datFilename) :
      FileBasedVolume(datFilename),
      m_raw(nullptr)
    {
    }

    QVisVolume::QVisVolume(const std::string& datFilename,
                           const std::string& rawFilename,
                           VolumeMetadataPtr data) :
      FileBasedVolume(datFilename, std::make_shared<QVisVolumeMetadata>(data, rawFilename)),
      m_raw( std::make_shared<RawVolume>(rawFilename,m_metadata))
    {
    }

    void QVisVolume::load(bool bReadwrite) {
      QVisVolumeMetadataPtr md = std::make_shared<QVisVolumeMetadata>(m_filename);
      m_metadata = md;
      m_raw = std::make_shared<RawVolume>(md->getRawFilename(), m_metadata);
      m_raw->load(bReadwrite);
      m_dataState = bReadwrite ? DS_CanReadWrite : DS_CanReadOnly;
    }

    void QVisVolume::create() {
      assert(m_raw);
      assert(m_metadata);

      QVisVolumeMetadataPtr md = std::static_pointer_cast<QVisVolumeMetadata>(m_metadata);
      md->save(m_filename);

      m_raw->create();
      m_dataState = DS_CanReadWrite;
    }

    void QVisVolume::save() {
      assert(m_raw);
      m_raw->save();
    }

    const std::string QVisVolume::toString() const {
      std::stringstream result;
      result << "QVis Dataset " << FileBasedVolume::toString();
      return result.str();
    }

  }
}

/*
 The MIT License

 Copyright (c) 2014 HPC Group, Univeristy Duisburg-Essen

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
