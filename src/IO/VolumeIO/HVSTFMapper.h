#ifndef HVSTFMAPPER_H
#define HVSTFMAPPER_H

#include <Core/SilverBulletBase.h>
#include "TextfileTFMapper.h"

namespace DataIO {
  namespace VolumeIO {
    class HVSTFMapper : public TextfileTFMapper {
    public:

      virtual bool load(const std::string& filename) {
        std::ifstream tf(filename.c_str());

        // skip first two lines
        char dummy[256];
        tf.getline(dummy, 256);
        tf.getline(dummy, 256);

        // always assume 8bit
        m_pvColorData.resize(256);

        return loadStream(tf);
      }
    };

    typedef std::shared_ptr<HVSTFMapper> HVSTFMapperPtr;
  }
}

#endif // HVSTFMAPPER_H

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
