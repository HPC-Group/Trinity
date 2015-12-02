#ifndef DATATYPE_H
#define DATATYPE_H

#include <silverbullet/base/SilverBulletBase.h>

#include <algorithm>

namespace DataIO {
  class DataType : public Core::Streamable {
  public:
    DataType();
    DataType(bool _signed, bool _float, uint32_t _bytes);
    
    enum ETypename {
      TN_FLOAT,
      TN_DOUBLE,
      TN_UINT64,
      TN_UINT32,
      TN_UINT16,
      TN_UINT8,
      TN_INT64,
      TN_INT32,
      TN_INT16,
      TN_INT8,
      TN_UNKNOWN,
    };
    
    template<typename T>
    bool checkType() {
      return (m_signed == std::is_signed<T>::value &&
              m_float == std::is_floating_point<T>::value &&
              m_bytes == sizeof(T));
    }
    
    const std::string toString() const;
    
    bool operator ==( const DataType& other ) const;
    bool operator !=( const DataType& other ) const;
    double getMin() const;
    double getMax() const;
    ETypename getTypeID() const;
    
    static DataType getCommonSupertype(const DataType t1,
                                       const DataType t2) {
      return DataType(t1.m_signed || t2.m_signed,
                      t1.m_float || t2.m_float,
                      std::max(t1.m_bytes, t2.m_bytes));
    }
    
    bool m_signed;
    bool m_float;
    uint32_t m_bytes;
    
  };
}

#endif // DATATYPE_H

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
