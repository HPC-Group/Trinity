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

/**
  \file    TransferFunction1D.h
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \version 1.0
  \date    September 2008
*/
#pragma once

#ifndef TRANSFERFUNCTION1D
#define TRANSFERFUNCTION1D

#include <string>
#include <vector>
#include <memory>
#include <Core/Math/Vectors.h>
#include <Core/Math/Grids.h>
using namespace Core::Math;

typedef Core::Math::VECTOR2<uint64_t> UINT64VECTOR2;
//typedef Core::Math::Grid1D<uint32_t> Histogram1D; //! \todo fix
//typedef Core::Math::Grid1D<float> NormalizedHistogram1D; //! \todo fix

namespace DataIO{
class TransferFunction1D
{
public:
  TransferFunction1D(size_t iSize = 0);
  TransferFunction1D(const std::string& filename);
  ~TransferFunction1D(void);

  void SetStdFunction(float fCenterPoint=0.5f, float fInvGradient=0.5f);
  void SetStdFunction(float fCenterPoint, float fInvGradient, int iComponent, bool bInvertedStep);
  /// Set the transfer function from an external source.  Assumes the vector
  /// has 4-components per element, in RGBA order.
  void Set(const std::vector<unsigned char>&);
  void Set(const std::vector<Core::Math::Vec4f>& d){
	  m_pvColorData.reset();
	  m_pvColorData = std::make_shared<std::vector<Core::Math::Vec4f>>(d);
	  ComputeNonZeroLimits();
  }

  size_t GetSize() const {return m_pvColorData->size();}
  void Resize(size_t iSize);
  void Resample(size_t iTargetSize);
  void FillOrTruncate(size_t iTargetSize);

  std::shared_ptr<std::vector<Core::Math::Vec4f>> GetColorData();
  Core::Math::Vec4f GetColor(size_t index) const;
  void SetColor(size_t index, Core::Math::Vec4f color);

  bool Load(const std::string& filename);
  bool Load(const std::string& filename, size_t iTargetSize);
  bool Load(std::istream& file);
  bool Load(std::istream& file, size_t iTargetSize);
  bool Save(std::ostream& file) const;
  bool Save(const std::string& filename) const;

  void Clear();

  void GetByteArray(std::vector<unsigned char>& vData,
                    unsigned char cUsedRange = 255) const;
  void GetShortArray(unsigned short** psData,
                     unsigned short sUsedRange=4095) const;
  void GetFloatArray(float** pfData) const;

  void ComputeNonZeroLimits();
  const Core::Math::Vec2ui64& GetNonZeroLimits() { return m_vValueBBox; }

private:
  Core::Math::Vec2ui64 m_vValueBBox;

  /// m_pvColorData exists as a shared pointer so that it can be handed off in
  /// an efficient manner to C++ code (from Lua inside of Tuvok).
  std::shared_ptr<std::vector<Core::Math::Vec4f>> m_pvColorData;

  float Smoothstep(float x) const;
};
}
#endif // TRANSFERFUNCTION1D
