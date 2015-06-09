#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>
#include <core/SilverBulletBase.h>
#include <core/Math/Vectors.h>
#include "GeometryMetadata.h"
#include "GeometryError.h"

namespace DataIO {
  namespace GeometryIO {

    class Geometry : public Core::Streamable {
    public:
      Geometry();
      Geometry(GeometryMetadataPtr metadata);

      virtual void load(bool bReadwrite) = 0;
      virtual void save() = 0;
      virtual void create() = 0;
      virtual const std::string toString() const;

      const GeometryMetadataPtr getMetadata() const {return m_metadata;}

      virtual void getPoint(uint64_t index, Core::Math::Vec3f& p) = 0;
      Core::Math::Vec3f getPoint(uint64_t index) {
        Core::Math::Vec3f v;
        getPoint(index,v);
        return v;
      }

      virtual void getPrimitive(uint64_t index, std::vector<uint64_t>& p) = 0;
      std::vector<uint64_t> getPrimitive(uint64_t index) {
        std::vector<uint64_t> v;
        getPrimitive(index,v);
        return v;
      }

      template <typename T> T getAttribute(uint64_t index, uint32_t i) {
        switch (m_metadata->getAttributes()[i].getType().getTypeID()) {
          case DataType::TN_FLOAT   :
            return convertAttribute<T,float>(index, i);
          case DataType::TN_DOUBLE  :
            return convertAttribute<T,double>(index, i);
          case DataType::TN_UINT64  :
            return convertAttribute<T,uint64_t>(index, i);
          case DataType::TN_UINT32  :
            return convertAttribute<T,uint32_t>(index, i);
          case DataType::TN_UINT16  :
            return convertAttribute<T,uint16_t>(index, i);
          case DataType::TN_UINT8   :
            return convertAttribute<T,uint8_t>(index, i);
          case DataType::TN_INT64   :
            return convertAttribute<T,int64_t>(index, i);
          case DataType::TN_INT32   :
            return convertAttribute<T,int32_t>(index, i);
          case DataType::TN_INT16   :
            return convertAttribute<T,int16_t>(index, i);
          case DataType::TN_INT8    :
            return convertAttribute<T,int8_t>(index, i);
          case DataType::TN_UNKNOWN : {
            std::stringstream error;
            error << "invalid data format ("<< m_metadata->getAttributes()[i].getType().toString() << ") in attribute " << i;
            throw GeometryError(error.str());
          }
        }
      }


    protected:
      enum EDataState {
        DS_Unavailable,
        DS_CanReadOnly,
        DS_CanReadWrite
      };

      EDataState m_dataState;
      GeometryMetadataPtr m_metadata;

      template <typename T, typename U> T convertAttribute(uint64_t index, uint32_t i) {
        U data;
        getAttributeAsBytes(index, i, (uint8_t*)&data);
        return (T)data;
      }

      virtual void getAttributeAsBytes(uint64_t index, uint32_t i, uint8_t* data) = 0;

    };
  }

}

#endif // GEOMETRY_H

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
