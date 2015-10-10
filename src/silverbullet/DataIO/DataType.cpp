#include "DataType.h"

#include <limits>
#include <sstream>

namespace DataIO {
  DataType::DataType() :
  m_signed(false),
  m_float(false),
  m_bytes(0)
  {}
  
  DataType::DataType(bool _signed, bool _float, uint32_t _bytes) :
  m_signed(_signed),
  m_float(_float),
  m_bytes(_bytes)
  {}
  
  const std::string DataType::toString() const {
    std::stringstream result;
    result << m_bytes*8 << " bits";
    
    if (!m_float) {
      result << ", " << (m_signed ? "signed" : "unsigned") << " integer";
    } else {
      result << ", float";
    }
    
    return result.str();
  }
  
  
  bool DataType::operator ==( const DataType& other ) const {
    return (m_signed == other.m_signed &&
            m_float == other.m_float &&
            m_bytes == other.m_bytes);
  }
  
  bool DataType::operator !=( const DataType& other ) const {
    return ! (*this == other);
  }
  
  double DataType::getMin() const {
    if (m_float) {
      switch (m_bytes) {
        case 4 : return std::numeric_limits<float>::max();
        case 8 : return std::numeric_limits<double>::max();
      }
    } else {
      if (m_signed) {
        switch (m_bytes) {
          case 1 : return double(std::numeric_limits<int8_t>::min());
          case 2 : return double(std::numeric_limits<int16_t>::min());
          case 4 : return double(std::numeric_limits<int32_t>::min());
          case 8 : return double(std::numeric_limits<int64_t>::min());
        }
      } else {
        return 0;
      }
    }
    return 0; // only reached for invalid data types
  }
  
  double DataType::getMax() const {
    return double(std::numeric_limits<double>::max());
  }
  
  DataType::ETypename DataType::getTypeID() const {
    if (m_float) {
      switch (m_bytes) {
        case 4 : return TN_FLOAT;
        case 8 : return TN_DOUBLE;
      }
    } else {
      if (m_signed) {
        switch (m_bytes) {
          case 1 : return TN_INT8;
          case 2 : return TN_INT16;
          case 4 : return TN_INT32;
          case 8 : return TN_INT64;
        }
      } else {
        switch (m_bytes) {
          case 1 : return TN_UINT8;
          case 2 : return TN_UINT16;
          case 4 : return TN_UINT32;
          case 8 : return TN_UINT64;
        }
        
      }
      
    }
    return TN_UNKNOWN;
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
