#include <sstream>
#include "Geometry.h"

namespace DataIO {
  namespace GeometryIO {
    
    Geometry::Geometry() :
    m_dataState(DS_Unavailable),
    m_metadata(nullptr)
    {
    }
    
    Geometry::Geometry(GeometryMetadataPtr metadata) :
    m_dataState(DS_Unavailable),
    m_metadata(metadata)
    {
    }
    
    const std::string Geometry::toString() const {
      if (!m_metadata) {
        return "invalid metadata";
      }
      
      std::stringstream result;
      switch (m_dataState) {
        case DS_Unavailable : return "data is unavailable";
        case DS_CanReadOnly : result << "data is read only, "; break;
        case DS_CanReadWrite : result << "data supports read/write, "; break;
      }
      
      result << *m_metadata;
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
