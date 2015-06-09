#ifndef GEOMETRYMETADATA_H
#define GEOMETRYMETADATA_H

#include <string>
#include <vector>
#include <core/SilverBulletBase.h>
#include "../Metadata.h"
#include <IO/DataType.h>

namespace DataIO {
  namespace GeometryIO {

    class GeometryMetadata;
    typedef std::shared_ptr<GeometryMetadata> GeometryMetadataPtr;

    class Attribute {
    public:
      Attribute(std::string desc,
                DataType dataType,
                uint32_t components);
      virtual const std::string toString() const;

      const DataType& getType() const {return m_dataType;}
      const std::string& getDesc() const {return m_desc;}
      const uint32_t getComponents() const {return m_components;}

    private:
      std::string m_desc;
      DataType m_dataType;
      uint32_t m_components;
    };

    class GeometryMetadata : public Metadata {
    public:
      enum EPrimitiveType {
        PT_POINTS,
        PT_LINES,
        PT_POLYGONS,
        PT_TRISTRIPS
      };

      GeometryMetadata(uint64_t numPoints,
                       uint64_t numPrimitives,
                       bool bHasPointData,
                       EPrimitiveType primType,
                       const std::vector<Attribute>& attributes,
                       LicensePtr license = LicensePtr(new License()),
                       const std::string& desc = "",
                       const std::string& source = "");

      GeometryMetadata(const GeometryMetadataPtr other);
      virtual const std::string toString() const;
      uint64_t getNumPoints() const {return m_numPoints;}
      uint64_t getNumPrimitives() const {return m_numPrimitives;}
      const std::vector<Attribute>& getAttributes() const {return m_attributes;}
      EPrimitiveType getPrimitiveType() const {return m_primType;}
      bool hasPointData() const {return m_bHasPointData;}

    protected:
      uint64_t m_numPoints;
      uint64_t m_numPrimitives;
      bool m_bHasPointData;
      EPrimitiveType m_primType;
      std::vector<Attribute> m_attributes;

      std::string primTypeToName() const;

    };
  }
}

#endif // GEOMETRYMETADATA_H

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
