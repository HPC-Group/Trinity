#include "GeometryMetadata.h"

#include <sstream>

namespace DataIO {
  namespace GeometryIO {
    
    Attribute::Attribute(std::string desc,
                         DataType dataType,
                         uint32_t components) :
      m_desc(desc),
      m_dataType(dataType),
      m_components(components)
    {
    }
    
    const std::string Attribute::toString() const {
      std::stringstream result;
      result << m_desc << " with " << m_components
      << " component(s) stored as " << m_dataType.toString() << "; " ;
      return result.str();
      
    }
    
    GeometryMetadata::GeometryMetadata(uint64_t numPoints,
                                       uint64_t numPrimitives,
                                       bool bHasPointData,
                                       EPrimitiveType primType,
                                       const std::vector<Attribute>& attributes,
                                       LicensePtr license,
                                       const std::string& desc,
                                       const std::string& source) :
    Metadata(license, desc, source),
    m_numPoints(numPoints),
    m_numPrimitives(numPrimitives),
    m_bHasPointData(bHasPointData),
    m_primType(primType),
    m_attributes(attributes)
    {
    }
    
    
    GeometryMetadata::GeometryMetadata(const GeometryMetadataPtr other) :
    Metadata(other->m_license, other->m_desc, other->m_source),
    m_numPoints(other->m_numPoints),
    m_numPrimitives(other->m_numPrimitives),
    m_bHasPointData(other->m_bHasPointData),
    m_primType(other->m_primType),
    m_attributes(other->m_attributes)
    {
    }
    
    
    std::string GeometryMetadata::primTypeToName() const {
      switch (m_primType) {
        case  PT_POINTS  : return "points";
        case  PT_LINES  : return "lines";
        case  PT_POLYGONS  : return "polygons";
        case  PT_TRISTRIPS  : return "triangle strips";
      }
    }
    
    const std::string GeometryMetadata::toString() const {
      std::stringstream result;
      result << "Number of points: " << m_numPoints << " with "
             << m_attributes.size() << " components ( ";
      
      for(const auto& a : m_attributes) {
        result << a.toString() << " ";
      }
      
      result << "), arranged as "
             << primTypeToName() << ", "
             << Metadata::toString();
      return result.str();
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
