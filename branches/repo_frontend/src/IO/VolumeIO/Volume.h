#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <core/SilverBulletBase.h>
#include "VolumeMetadata.h"
#include "VolumeError.h"
#include <cassert>

namespace DataIO {
  namespace VolumeIO {

    class Volume : public Core::Streamable {
    public:
      Volume();
      Volume(VolumeMetadataPtr metadata);

      virtual void create() = 0;

      virtual const std::string toString() const;

      template <typename T>
      T getValue(const Core::Math::Vec3ui64& pos, uint64_t comp) {
        assert(m_dataState != DS_Unavailable);
        assert(m_metadata->checkType<T>());

        // unfold all possible types, these  branches
        // are removed by the compiler as only one of them
        // is true at compile time
        if (typeid(float) == typeid(T)) return getFloatValue(pos, comp); else
        if (typeid(double) == typeid(T)) return getDoubleValue(pos, comp); else
        if (typeid(uint64_t) == typeid(T)) return getUint64Value(pos, comp); else
        if (typeid(uint32_t) == typeid(T)) return getUint32Value(pos, comp); else
        if (typeid(uint16_t) == typeid(T)) return getUint16Value(pos, comp); else
        if (typeid(uint8_t) == typeid(T)) return getUint8Value(pos, comp); else
        if (typeid(int64_t) == typeid(T)) return getInt64Value(pos, comp); else
        if (typeid(int32_t) == typeid(T)) return getInt32Value(pos, comp); else
        if (typeid(int16_t) == typeid(T)) return getInt16Value(pos, comp); else
        return getInt8Value(pos, comp); // only one type left (typeid(int8_t) == typeid(T))
      }


      template <typename T>
      void setValue(const Core::Math::Vec3ui64& pos, uint64_t comp, T val) {
        assert(m_dataState == DS_CanReadWrite);
        assert(m_metadata->checkType<T>());

        // unfold all possible types, these branches
        // are removed by the compiler as only one of them
        // is true at compile time
        if (typeid(float) == typeid(T)) setFloatValue(pos, comp, float(val)); else
        if (typeid(double) == typeid(T)) setDoubleValue(pos, comp, double(val)); else
        if (typeid(uint64_t) == typeid(T)) setUint64Value(pos, comp, uint64_t(val)); else
        if (typeid(uint32_t) == typeid(T)) setUint32Value(pos, comp, uint32_t(val)); else
        if (typeid(uint16_t) == typeid(T)) setUint16Value(pos, comp, uint16_t(val)); else
        if (typeid(uint8_t) == typeid(T)) setUint8Value(pos, comp, uint8_t(val)); else
        if (typeid(int64_t) == typeid(T)) setInt64Value(pos, comp, int64_t(val)); else
        if (typeid(int32_t) == typeid(T)) setInt32Value(pos, comp, int32_t(val)); else
        if (typeid(int16_t) == typeid(T)) setInt16Value(pos, comp, int16_t(val)); else
        setInt8Value(pos, comp, int8_t(val)); // only one type left (typeid(int8_t) == typeid(T))
      }

      const VolumeMetadataPtr getMetadata() const {return m_metadata;}

      virtual bool supportsContinousMemoryPointer() const {return false;}
      virtual const uint8_t* getContinousMemoryPointer() const {return nullptr;}

    protected:
      enum EDataState {
        DS_Unavailable,
        DS_CanReadOnly,
        DS_CanReadWrite
      };

      EDataState m_dataState;
      VolumeMetadataPtr m_metadata;

      virtual float getFloatValue(const Core::Math::Vec3ui64&, uint64_t ) {
        throw VolumeError("float data not available in this volume format");
      }
      virtual double getDoubleValue(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("double data not available in this volume format");
      }
      virtual uint64_t getUint64Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("unsigned int 64bit data not available in this volume format");
      }
      virtual uint32_t getUint32Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("unsigned int 32bit data not available in this volume format");
      }
      virtual uint16_t getUint16Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("unsigned int 16bit data not available in this volume format");
      }
      virtual uint8_t getUint8Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("unsigned int 8bit data not available in this volume format");
      }
      virtual int64_t getInt64Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("signed int 64bit data not available in this volume format");
      }
      virtual int32_t getInt32Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("signed int 32bit data not available in this volume format");
      }
      virtual int16_t getInt16Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("signed int 16bit data not available in this volume format");
      }
      virtual int8_t getInt8Value(const Core::Math::Vec3ui64&, uint64_t ){
        throw VolumeError("signed int 8bit data not available in this volume format");
      }
      virtual void setFloatValue(const Core::Math::Vec3ui64&, uint64_t , float ) {
        throw VolumeError("float data not available in this volume format");
      }
      virtual void setDoubleValue(const Core::Math::Vec3ui64&, uint64_t , double ){
        throw VolumeError("double data not available in this volume format");
      }
      virtual void setUint64Value(const Core::Math::Vec3ui64&, uint64_t , uint64_t ){
        throw VolumeError("unsigned int 64bit data not available in this volume format");
      }
      virtual void setUint32Value(const Core::Math::Vec3ui64&, uint64_t , uint32_t ){
        throw VolumeError("unsigned int 32bit data not available in this volume format");
      }
      virtual void setUint16Value(const Core::Math::Vec3ui64&, uint64_t , uint16_t ){
        throw VolumeError("unsigned int 16bit data not available in this volume format");
      }
      virtual void setUint8Value(const Core::Math::Vec3ui64&, uint64_t , uint8_t ){
        throw VolumeError("unsigned int 8bit data not available in this volume format");
      }
      virtual void setInt64Value(const Core::Math::Vec3ui64&, uint64_t , int64_t ){
        throw VolumeError("signed int 64bit data not available in this volume format");
      }
      virtual void setInt32Value(const Core::Math::Vec3ui64&, uint64_t , int32_t ){
        throw VolumeError("signed int 32bit data not available in this volume format");
      }
      virtual void setInt16Value(const Core::Math::Vec3ui64&, uint64_t , int16_t ){
        throw VolumeError("signed int 16bit data not available in this volume format");
      }
      virtual void setInt8Value(const Core::Math::Vec3ui64&, uint64_t , int8_t ){
        throw VolumeError("signed int 8bit data not available in this volume format");
      }

    };
    typedef std::shared_ptr<Volume> VolumePtr;
  }

}

#endif // VOLUMEM_H

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
