#ifndef VTKGEOMETRY_H
#define VTKGEOMETRY_H

#include <Core/SilverBulletBase.h>
#include "Geometry.h"
#include <Core/IO/MemMappedFile.h>

namespace DataIO {
  namespace GeometryIO {

    class VTKGeometry : public Geometry {
    public:
      VTKGeometry(const std::string& filename);
      VTKGeometry(const std::string& filename, GeometryMetadataPtr metadata);

      virtual void load(bool bReadwrite) override;
      virtual void save() override;
      virtual void create() override;
      virtual const std::string toString() const override;

      virtual void getPoint(uint64_t index, Core::Math::Vec3f& p) override;
      virtual void getPrimitive(uint64_t index, std::vector<uint64_t>& p) override;

    protected:
      std::string m_filename;
      Core::IO::MemMappedFilePtr m_vtkFile;

      uint64_t m_pointOffset;
      uint64_t m_topoOffset;
      std::vector<uint64_t> m_attribOffsets;

      void readHeader();

      virtual void getAttributeAsBytes(uint64_t index,
                                       uint32_t i, uint8_t* data) override;

      void parseAttributes(uint64_t& pos, std::vector<Attribute>& attributes,
                           uint32_t attribCount);
      void parsePoints(const std::string& sectionDesc,
                       const std::vector<std::string>& token,
                       uint64_t& numPoints, uint64_t& sectionSize);
      void parseTopology(const std::string& sectionDesc,
                         const std::vector<std::string>& token,
                         uint64_t& numPrimitives, uint64_t& sectionSize);
      void parseAttributeData(const std::string& sectionDesc,
                          const std::vector<std::string>& token,
                          uint64_t& pos, uint64_t numElems,
                          std::vector<Attribute>& attributes);

    };
  }

}

#endif // VTKGEOMETRY_H

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
