#ifndef RAWVOLUME_H
#define RAWVOLUME_H

#include <string>
#include <cassert>
#include "silverbullet/base/SilverBulletBase.h"

#include "FileBasedVolume.h"
#include "silverbullet/io/MemMappedFile.h"

namespace DataIO {
  namespace VolumeIO {
    
    class RawVolume : public FileBasedVolume {
    public:
      RawVolume(const std::string& rawFilename,
                VolumeMetadataPtr data);

      virtual bool supportsContinousMemoryPointer() const {return true;}
      virtual const uint8_t* getContinousMemoryPointer() const {
        assert(m_volume);
        return (uint8_t*)(m_volume->GetDataPointer());
      }
      uint64_t positionToIndex(Core::Math::Vec3ui64 pos, uint64_t c) const;

      virtual void load(bool bReadwrite);
      virtual void save();
      virtual void create();
      
      const std::string toString() const;
      
    protected:
      Core::IO::MemMappedFilePtr m_volume;
      
      virtual float getFloatValue(const Core::Math::Vec3ui64& p, uint64_t c) {
        return getMMValue<float>(p, c);
      }
      virtual double getDoubleValue(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<double>(p, c);
      }
      virtual uint64_t getUint64Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<uint64_t>(p, c);
      }
      virtual uint32_t getUint32Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<uint32_t>(p, c);
      }
      virtual uint16_t getUint16Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<uint16_t>(p, c);
      }
      virtual uint8_t getUint8Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<uint8_t>(p, c);
      }
      virtual int64_t getInt64Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<int64_t>(p, c);
      }
      virtual int32_t getInt32Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<int32_t>(p, c);
      }
      virtual int16_t getInt16Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<int16_t>(p, c);
      }
      virtual int8_t getInt8Value(const Core::Math::Vec3ui64& p, uint64_t c){
        return getMMValue<int8_t>(p, c);
      }

      virtual void setFloatValue(const Core::Math::Vec3ui64& p, uint64_t c, float value) {
        setMMValue<float>(p, c, value);
      }
      virtual void setDoubleValue(const Core::Math::Vec3ui64& p, uint64_t c, double value){
        setMMValue<double>(p, c, value);
      }
      virtual void setUint64Value(const Core::Math::Vec3ui64& p, uint64_t c, uint64_t value){
        setMMValue<uint64_t>(p, c, value);
      }
      virtual void setUint32Value(const Core::Math::Vec3ui64& p, uint64_t c, uint32_t value){
        setMMValue<uint32_t>(p, c, value);
      }
      virtual void setUint16Value(const Core::Math::Vec3ui64& p, uint64_t c, uint16_t value){
        setMMValue<uint16_t>(p, c, value);
      }
      virtual void setUint8Value(const Core::Math::Vec3ui64& p, uint64_t c, uint8_t value){
        setMMValue<uint8_t>(p, c, value);
      }
      virtual void setInt64Value(const Core::Math::Vec3ui64& p, uint64_t c, int64_t value){
        setMMValue<int64_t>(p, c, value);
      }
      virtual void setInt32Value(const Core::Math::Vec3ui64& p, uint64_t c, int32_t value){
        setMMValue<int32_t>(p, c, value);
      }
      virtual void setInt16Value(const Core::Math::Vec3ui64& p, uint64_t c, int16_t value){
        setMMValue<int16_t>(p, c, value);
      }
      virtual void setInt8Value(const Core::Math::Vec3ui64& p, uint64_t c, int8_t value){
        setMMValue<int8_t>(p, c, value);
      }
      
      template <typename T>
      T getMMValue(const Core::Math::Vec3ui64& p, uint64_t c) {
        uint64_t index = positionToIndex(p, c);
        return ((T*)(getContinousMemoryPointer()))[index];
      }

      template <typename T>
      void setMMValue(const Core::Math::Vec3ui64& p, uint64_t c, T value) {
        uint64_t index = positionToIndex(p, c);
        ((T*)(getContinousMemoryPointer()))[index] = value;
      }
      
    };
    typedef std::shared_ptr<RawVolume> RawVolumePtr;

  }
}

#endif // RAWVOLUME_H

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
