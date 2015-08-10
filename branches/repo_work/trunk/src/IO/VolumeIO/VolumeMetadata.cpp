#include "VolumeMetadata.h"

#include <sstream>

using namespace Core::Math;

namespace DataIO {
  namespace VolumeIO {
    
    VolumeMetadata::VolumeMetadata(Vec3ui64 vSize,
                                   DataType dataType,
                                   uint32_t iUsedbits,
                                   uint32_t iComponents,
                                   Vec3d vAspect,
                                   Mat4d mTransformation,
                                   LicensePtr license,
                                   const std::string& desc,
                                   const std::string& source) :
    Metadata(license, desc, source),
    m_size(vSize),
    m_dataType(dataType),
    m_usedbits(iUsedbits),
    m_components(iComponents),
    m_aspect(vAspect),
    m_transformation(mTransformation)
    {
    }
    
    
    VolumeMetadata::VolumeMetadata(const VolumeMetadataPtr other) :
    Metadata(other->m_license, other->m_desc, other->m_source),
    m_size(other->m_size),
    m_dataType(other->m_dataType),
    m_usedbits(other->m_usedbits),
    m_components(other->m_components),
    m_aspect(other->m_aspect),
    m_transformation(other->m_transformation)
    {
    }
    
    const std::string VolumeMetadata::toString() const {
      std::stringstream result;
      result << "Size: " << m_size;
      result << ", " << m_dataType;
      
      if (m_usedbits != m_dataType.m_bytes*8) {
        result << " (only " << m_usedbits << " used)";
      }
      
      if (m_components == 1)
        result << ", " << m_components << " component";
      else
        result << ", " << m_components << " components";
      
      if (m_aspect != Vec3d(1,1,1)) {
        result << ", aspect ratio " << m_aspect;
      }
      
      if (m_transformation != Mat4d()) {
        result << ", transformation " << m_transformation;
      }
      
      result << ", " << Metadata::toString();
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
