#ifndef METADATA_H
#define METADATA_H

#include <SilverBulletBase.h>
#include "License.h"

namespace DataIO {
  class Metadata : public Core::Streamable {
  public:
    Metadata(LicensePtr license = LicensePtr(new License()),
             const std::string& desc = "",
             const std::string& source = "");
    
    virtual const std::string toString() const;
    
    void setLicense(LicensePtr license) {
      m_license = license;
    }
    void setDesc(const std::string& desc) {
      m_desc = desc;
    }
    void setSource(const std::string& source) {
      m_source = source;
    }
    
    
  protected:
    LicensePtr m_license;
    std::string m_desc;
    std::string m_source;
    
  };
}

#endif // METADATA_H

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
