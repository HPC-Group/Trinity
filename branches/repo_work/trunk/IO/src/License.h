#ifndef LICENSE_H
#define LICENSE_H

#include <string>
#include <memory>
#include <core/SilverBulletBase.h>

class License : public Core::Streamable {
public:
  enum EShareRights{
    SR_Unknown,
    SR_PublicDomain,
    SR_Restricted,
    SR_Confidential
  };

  License(EShareRights rights = SR_Unknown, const std::string& desc = "");

  const std::string& getDescription() const {return m_desc;}
  const EShareRights& getShareRights() const {return m_rights;}

  const std::string toString() const;

private:
  std::string m_desc;
  EShareRights m_rights;
};

typedef std::shared_ptr<License> LicensePtr;

#endif // LICENSE_H

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
