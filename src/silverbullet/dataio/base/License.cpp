#include "silverbullet/dataio/base/License.h"

#include <sstream>

License::License(EShareRights rights, const std::string& desc) :
  m_desc(desc),
  m_rights(rights)
{
}
  
const std::string License::toString() const {
  std::stringstream result;
  
  result << "description: \"" << m_desc << "\" rights: ";
  
  switch (m_rights) {
    case SR_PublicDomain : result << "public domain"; break;
    case SR_Restricted : result << "restricted"; break;
    case SR_Confidential : result << "confidential"; break;
    case SR_Unknown : // fall throught on purpose
    default: result << "unknown"; break;
  }
  
  return result.str();
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
