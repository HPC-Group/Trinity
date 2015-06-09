/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


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
#pragma once

#ifndef TUVOK_MASTERCONTROLLER_H
#define TUVOK_MASTERCONTROLLER_H

#include <core/StdTuvokDefines.h>
#include <tools/Singleton.h>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

class SystemInfo;

namespace Tuvok {

class BasicsController {
    SINGLETON_IVDA_HEAP(BasicsController)

public:

  /// Defaults to using a Console-based debug output stream.
  virtual ~BasicsController();

  /// Deallocate any memory we're holding; rendering or doing any real work is
  /// undefined after this operation.
  void Cleanup();

  /// System information is for looking up host parameters, such as the
  /// amount of memory available.
  ///@{
  SystemInfo*       SysInfo()       { return m_pSystemInfo; }
  const SystemInfo& SysInfo() const { return *m_pSystemInfo; }
  ///@}

  /// Whether or not to expose certain features which aren't actually ready for
  /// users.
  bool ExperimentalFeatures() const;
  void ExperimentalFeatures(bool b);

  void SetMaxCPUMem(uint64_t megs);

  /// return max mem in megabyte
  uint64_t GetMaxGPUMem() const;
  uint64_t GetMaxCPUMem() const;

  /// centralized storage for renderer parameters
  ///@{
  void SetBrickStrategy(size_t strat);
  void SetRehashCount(uint32_t count);
  void SetMDUpdateStrategy(unsigned); ///< takes DM_* enum
  void SetHTSize(unsigned); ///< hash table size

  size_t GetBrickStrategy() const;
  uint32_t GetRehashCount() const;
  unsigned GetMDUpdateStrategy() const;
  unsigned GetHTSize() const;
  ///@}

private:
  SystemInfo*      m_pSystemInfo;
  bool             m_bDeleteDebugOutOnExit;
  bool             m_bExperimentalFeatures;
};

}
#endif // TUVOK_MASTERCONTROLLER_H
