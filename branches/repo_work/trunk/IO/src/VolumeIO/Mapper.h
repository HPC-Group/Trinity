#ifndef MAPPER_H
#define MAPPER_H

#include <core/SilverBulletBase.h>

namespace DataIO {
  namespace VolumeIO {

    class Mapper {
    public:

      virtual uint32_t getComponents() const = 0;

      template<typename T>
      std::vector<double> map(T val) {
        // unfold all possible types, these branches
        // are removed by the compiler as only one of them
        // is true during compile time
        if (typeid(float) == typeid(T)) return mapFloatValue(float(val));
        if (typeid(double) == typeid(T)) return mapDoubleValue(double(val));
        if (typeid(uint64_t) == typeid(T)) return mapUint64Value(uint64_t(val));
        if (typeid(uint32_t) == typeid(T)) return mapUint32Value(uint32_t(val));
        if (typeid(uint16_t) == typeid(T)) return mapUint16Value(uint16_t(val));
        if (typeid(uint8_t) == typeid(T)) return mapUint8Value(uint8_t(val));
        if (typeid(int64_t) == typeid(T)) return mapInt64Value(int64_t(val));
        if (typeid(int32_t) == typeid(T)) return mapInt32Value(int32_t(val));
        if (typeid(int16_t) == typeid(T)) return mapInt16Value(int16_t(val));
        // only one type left
        //if (typeid(int8_t) == typeid(T))
        return mapInt8Value(int8_t(val));
      }

    protected:
      virtual std::vector<double> mapFloatValue(float ) {
        throw VolumeError("float data not available in this mapper");
      }
      virtual std::vector<double> mapDoubleValue(double ){
        throw VolumeError("double data not available in this mapper");
      }
      virtual std::vector<double> mapUint64Value(uint64_t ){
        throw VolumeError("unsigned int 64bit data not available in this mapper");
      }
      virtual std::vector<double> mapUint32Value(uint32_t ){
        throw VolumeError("unsigned int 32bit data not available in this mapper");
      }
      virtual std::vector<double> mapUint16Value(uint16_t ){
        throw VolumeError("unsigned int 16bit data not available in this mapper");
      }
      virtual std::vector<double> mapUint8Value(uint8_t ){
        throw VolumeError("unsigned int 8bit data not available in this mapper");
      }
      virtual std::vector<double> mapInt64Value(int64_t ){
        throw VolumeError("signed int 64bit data not available in this mapper");
      }
      virtual std::vector<double> mapInt32Value(int32_t ){
        throw VolumeError("signed int 32bit data not available in this mapper");
      }
      virtual std::vector<double> mapInt16Value(int16_t ){
        throw VolumeError("signed int 16bit data not available in this mapper");
      }
      virtual std::vector<double> mapInt8Value(int8_t ){
        throw VolumeError("signed int 8bit data not available in this mapper");
      }
    };
    typedef std::shared_ptr<Mapper> MapperPtr;
  }
}

#endif // MAPPER_H

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
