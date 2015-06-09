#ifndef IV3DTFMAPPER_H
#define IV3DTFMAPPER_H

#include <core/SilverBulletBase.h>
#include "TextfileTFMapper.h"
#include "Volume.h"

namespace DataIO {
  namespace VolumeIO {
    class IV3DTFMapper : public TextfileTFMapper {
    public:

      virtual bool load(const std::string& filename, uint64_t maxVal) {
        std::ifstream tf(filename.c_str());
        uint32_t iSize;
        tf >> iSize;
        if(!tf) {
          return false;
        }
        m_pvColorData.resize(iSize);

        if (!loadStream(tf)) return false;

        if (maxVal >= getSize()) {
          resample(size_t(maxVal+1));
        }

        return true;

      }
    };

    typedef std::shared_ptr<IV3DTFMapper> IV3DTFMapperPtr;
  }
}

#endif // IV3DTFMAPPER_H

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
