#ifndef VOLUMEMETADATA_H
#define VOLUMEMETADATA_H

#include <string>
#include <type_traits>
#include <Core/SilverBulletBase.h>

#include <IO/Metadata.h>
#include <IO/DataType.h>
#include <Core/Math/Vectors.h>

namespace DataIO {
  namespace VolumeIO {

    class VolumeMetadata;
    typedef std::shared_ptr<VolumeMetadata> VolumeMetadataPtr;

    class VolumeMetadata : public Metadata {
    public:
      VolumeMetadata(Core::Math::Vec3ui64 vSize = Core::Math::Vec3ui64(0,0,0),
                     DataType dataType = DataType(),
                     uint32_t iUsedbits = 0,
                     uint32_t iComponents = 0,
                     Core::Math::Vec3d vAspect = Core::Math::Vec3d(1,1,1),
                     Core::Math::Mat4d mTransformation = Core::Math::Mat4d(),
                     LicensePtr license = LicensePtr(new License()),
                     const std::string& desc = "",
                     const std::string& source = "");

      VolumeMetadata(const VolumeMetadataPtr other);

      virtual const std::string toString() const;

      template<typename T>
      bool checkType() {
        return m_dataType.checkType<T>();
      }

      const Core::Math::Vec3ui64& getSize() const {return m_size;}
      const DataType& getType() const {return m_dataType;}
      const uint32_t getUsedBits() const {return m_usedbits;}
      const uint32_t getComponents() const {return m_components;}
      const Core::Math::Vec3d& getAspectRatio() const {return m_aspect;}
      const Core::Math::Mat4d& getTransformation() const {return m_transformation;}

    protected:
      Core::Math::Vec3ui64 m_size;
      DataType m_dataType;
      uint32_t m_usedbits;
      uint32_t m_components;
      Core::Math::Vec3d m_aspect;
      Core::Math::Mat4d m_transformation;
    };
  }
}

#endif // VOLUMEMETADATA_H

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
