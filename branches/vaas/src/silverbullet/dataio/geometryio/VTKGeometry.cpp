#include <sstream>

#include "VTKGeometry.h"
#include <Core/StringTools.h>
#include <Core/Math/EndianConvert.h>

using namespace Core::Math;

namespace DataIO {
  namespace GeometryIO {
    
    VTKGeometry::VTKGeometry(const std::string& filename) :
    Geometry(),
    m_filename(filename),
    m_pointOffset(0),
    m_topoOffset(0)
    {
    }
    
    VTKGeometry::VTKGeometry(const std::string& filename,
                            GeometryMetadataPtr metadata) :
    Geometry(metadata),
    m_filename(filename),
    m_pointOffset(0),
    m_topoOffset(0)
    {
    }
    
    const std::string VTKGeometry::toString() const {
      std::stringstream result;
      result << "VTK Geometry Dataset " << Geometry::toString();
      return result.str();
    }
    
    void VTKGeometry::load(bool bReadwrite) {
      if (bReadwrite)
        throw GeometryError("currently vtk file support is read only");
      
      m_vtkFile = std::make_shared<Core::IO::MemMappedFile>(m_filename, bReadwrite ? Core::IO::MMFILE_ACCESS_READWRITE : Core::IO::MMFILE_ACCESS_READONLY);
      
      if (!m_vtkFile->IsOpen()) {
        std::stringstream error;
        error << "unable to open vtk file " << m_filename;
        throw GeometryError(error.str());
      }
      m_dataState = bReadwrite ? DS_CanReadWrite : DS_CanReadOnly;

      readHeader();
    }
    
    void VTKGeometry::save() {
      throw GeometryError("currently vtk file support is read only");
    }
    
    void VTKGeometry::create() {
      throw GeometryError("currently vtk file support is read only");
    }
    
    static std::string readLine(Core::IO::MemMappedFilePtr file, uint64_t& pos) {

      const uint64_t size = file->GetFileMappingSize();
      const uint8_t* charData = (uint8_t*)(file->GetDataPointer());

      bool endFound = false;
      std::string data;

      while (pos < size) {
        if (charData[pos] == '\n') {
          endFound =true;
          pos++;
          break;
        }
        data += charData[pos];
        pos++;
      }
      
      if (endFound)
        return data;
      else
        return "";
    }
    
    static DataType stringToDatatype(const std::string& str) {
      if (str == "double")
        return DataType(true, true, 8);
      if (str == "float")
        return DataType(true, true, 4);
      if (str == "unsigned_char")
        return DataType(false, false, 1);
      if (str == "char")
        return DataType(true, false, 1);
      if (str == "unsigned_short")
        return DataType(false, false, 2);
      if (str == "short")
        return DataType(true, false, 2);
      if (str == "unsigned_int")
        return DataType(false, false, 4);
      if (str == "int")
        return DataType(true, false, 4);
      if (str == "unsigned_long")
        return DataType(false, false, 8);
      if (str == "long")
        return DataType(true, false, 8);
      
      
      std::stringstream error;
      error << "datatype " << str << " not supported by this reader";
      throw GeometryError(error.str());
    }
    
    void VTKGeometry::parseAttributes(uint64_t& pos,
                                      std::vector<Attribute>& attributes,
                                      uint32_t attribCount) {
      
      attributes.clear();
      attributes.reserve(attribCount);
      m_attribOffsets.clear();
      m_attribOffsets.reserve(attribCount);
      
      for (size_t i = 0;i<attribCount;++i) {
        std::string fieldDesc = readLine(m_vtkFile, pos);
        std::vector<std::string> token = Core::StringTools::Tokenize(fieldDesc);
        
        if (token.size() != 4) {
          std::stringstream error;
          error << "invalid field description found " << fieldDesc;
          throw GeometryError(error.str());
        }
        
        std::string desc = token[0];
        uint32_t components = Core::StringTools::FromString<uint64_t>(token[1]);
        DataType dataType = stringToDatatype(token[3]);
        
        Attribute a(desc, dataType, components);
        attributes.push_back(a);
        
        m_attribOffsets.push_back(pos);
        
        pos += Core::StringTools::FromString<uint64_t>(token[2]) * dataType.m_bytes+1;
      }
    }

    void VTKGeometry::parsePoints(const std::string& sectionDesc,
                                  const std::vector<std::string>& token,
                                  uint64_t& numPoints, uint64_t& sectionSize) {
      if (token.size() != 3) {
        std::stringstream error;
        error << "invalid section description found " << sectionDesc << ", expected three tokens but got " << token.size();
        throw GeometryError(error.str());
      }
      
      sectionSize = Core::StringTools::FromString<uint64_t>(token[1]);
      
      if (token[2] != "float") {
        std::stringstream error;
        error << "currently this reader supports only float positions not " << sectionDesc;
        throw GeometryError(error.str());
      }
      
      numPoints = sectionSize;
      sectionSize = numPoints*4*3;
    }
    
    void VTKGeometry::parseTopology(const std::string& sectionDesc,
                                    const std::vector<std::string>& token,
                                    uint64_t& numPrimitives,
                                    uint64_t& sectionSize) {
      if (token.size() != 3) {
        std::stringstream error;
        error << "invalid section description found " << sectionDesc << ", expected three tokens but got " << token.size();
        throw GeometryError(error.str());
      }
      
      numPrimitives = Core::StringTools::FromString<uint64_t>(token[1]);
      sectionSize = Core::StringTools::FromString<uint64_t>(token[2])*4;
    }


    void VTKGeometry::parseAttributeData(const std::string& sectionDesc,
                                     const std::vector<std::string>& token,
                                     uint64_t& pos, uint64_t numElems,
                                     std::vector<Attribute>& attributes) {
      
      uint64_t numValues = Core::StringTools::FromString<uint64_t>(token[1]);
      
      if (numValues != numElems) {
        std::stringstream error;
        error << "invalid data description found " << sectionDesc
        << " number of point values (" << numValues
        << ") differs from number of points (" << numElems << ")";
        throw GeometryError(error.str());
      }
      
      std::string fieldDesc = readLine(m_vtkFile, pos);
      std::vector<std::string> fieldtoken = Core::StringTools::Tokenize(fieldDesc);
      
      if (fieldtoken[0] == "FIELD") {
        uint32_t attribCount = Core::StringTools::FromString<uint64_t>(fieldtoken[2]); 
        parseAttributes(pos, attributes, attribCount);
      } else {
        std::stringstream error;
        error << "invalid field description found ->" << fieldDesc << "<-";
        throw GeometryError(error.str());
      }
    }
    

    
    void VTKGeometry::readHeader() {
      uint64_t numPoints;
      uint64_t numPrimitives;
      bool bHasPointData;
      GeometryMetadata::EPrimitiveType primType = GeometryMetadata::PT_POINTS;
      
      std::vector<Attribute> attributes;
      std::string desc = "";
      
      // version number string
      uint64_t pos = 0;
      std::string versionInfo = readLine(m_vtkFile, pos);
      const std::string expectedText = "# vtk DataFile Version";
      auto res = std::mismatch(expectedText.begin(), expectedText.end(), versionInfo.begin());
      if (res.first != expectedText.end()) {
        throw GeometryError("this ist not a valid vtk legacy format file");
      }
      
      // "header", i.e., info about the dataset
      desc = readLine(m_vtkFile, pos);

      // file type
      std::string fileType = readLine(m_vtkFile, pos);
      if (Core::StringTools::ToUpperCase(fileType) != "BINARY") {
        std::stringstream error;
        error << "this dataset is in " << fileType << " format but currently this reader supports only BINARY";
        throw GeometryError(error.str());
      }
      
      std::string datasetType = readLine(m_vtkFile, pos);
      if (Core::StringTools::ToUpperCase(datasetType) != "DATASET POLYDATA") {
        std::stringstream error;
        error << "this reader only reads DATASET POLYDATA vtk files but the file contains " << fileType << ".";
        throw GeometryError(error.str());
      }

      
      while (pos < m_vtkFile->GetFileMappingSize()) {
        std::string sectionDesc = readLine(m_vtkFile, pos);
        
        std::vector<std::string> token = Core::StringTools::Tokenize(sectionDesc);
        
        if (token.size() < 2) {
          std::stringstream error;
          error << "invalid data description found " << sectionDesc << ", expected two tokens but got " << token.size();
          throw GeometryError(error.str());
        }
        
        uint64_t sectionSize = 0;
        
        if (token[0] == "POINTS") {
          parsePoints(sectionDesc, token, numPoints, sectionSize);
          m_pointOffset = pos;
          pos += sectionSize+1;
        } else if (token[0] == "LINES") {
          primType = GeometryMetadata::PT_LINES;
          parseTopology(sectionDesc, token, numPrimitives, sectionSize);
          m_topoOffset = pos;
          pos += sectionSize+1;
        } else if (token[0] == "POLYGONS") {
          primType = GeometryMetadata::PT_POLYGONS;
          parseTopology(sectionDesc, token, numPrimitives, sectionSize);
          m_topoOffset = pos;
          pos += sectionSize+1;
        } else if (token[0] == "TRIANGLE_STRIPS") {
          primType = GeometryMetadata::PT_TRISTRIPS;
          parseTopology(sectionDesc, token, numPrimitives, sectionSize);
          m_topoOffset = pos;
          pos += sectionSize+1;
        } else if (token[0] == "POINT_DATA") {
          parseAttributeData(sectionDesc, token, pos, numPoints, attributes);
          bHasPointData = true;
        } else if (token[0] == "CELL_DATA") {
          parseAttributeData(sectionDesc, token, pos, numPrimitives, attributes);
          bHasPointData = false;
        } else {
          std::stringstream error;
          error << "unknown data description found  " << sectionDesc;
          throw GeometryError(error.str());
        }
      }
      

      
      m_metadata = std::make_shared<GeometryMetadata>(numPoints,
                                                      numPrimitives,
                                                      bHasPointData,
                                                      primType,
                                                      attributes,
                                                      LicensePtr(new License()),
                                                      desc,
                                                      versionInfo);
    }

    template <typename T> T toBE(T data) {
      if (EndianConvert::IsBigEndian())
        return data;
      else
        return EndianConvert::Swap(data);
    }
    
    void VTKGeometry::getPoint(uint64_t index, Core::Math::Vec3f& p) {
      const uint8_t* charData = (uint8_t*)(m_vtkFile->GetDataPointer())+m_pointOffset;
      const float* floatData = (float*)charData;

      p.x = toBE(*(floatData+index*3+0));
      p.y = toBE(*(floatData+index*3+1));
      p.z = toBE(*(floatData+index*3+2));
    }
    
    void VTKGeometry::getPrimitive(uint64_t index, std::vector<uint64_t>& v) {
      const uint8_t* charData = (uint8_t*)(m_vtkFile->GetDataPointer())+m_topoOffset;
      const uint32_t* intData = (uint32_t*)charData;
      
      for (uint64_t i = 0;i<index;++i) {
        const uint32_t s = toBE(*intData);
        intData += s+1;
      }
      
      const uint32_t s = toBE(*intData);
      intData++;
      v.resize(s);
      for (uint32_t i = 0;i<s;++i) {
        v[i] = toBE(*intData);
        intData++;
      }
      
    }
    
    void VTKGeometry::getAttributeAsBytes(uint64_t index, uint32_t i, uint8_t* data) {
      const Attribute& attrib = m_metadata->getAttributes()[i];
      
      const uint8_t* dataStart = (uint8_t*)(m_vtkFile->GetDataPointer())+m_attribOffsets[i];
      const uint32_t elemSize = attrib.getType().m_bytes;
      
      std::copy(dataStart+elemSize*index, dataStart+elemSize*(index+1), data);
      
      if (!EndianConvert::IsBigEndian())
        EndianConvert::Swap((char*)data, elemSize, 1);
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
