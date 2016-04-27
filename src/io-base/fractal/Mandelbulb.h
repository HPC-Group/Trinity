#pragma once

#include <cmath>   // sin, etc.
#include <cstdint> // uint32_t etc.
#include <limits>  // numeric_limits

template <typename T> class Mandelbulb {
public:
  Mandelbulb(uint64_t totalX, uint64_t totalY, uint64_t totalZ,
             double bulbSize = 2.25,
             T iMaxIterations = std::numeric_limits<T>::max(),
             double fBailout = 100.0, uint32_t n = 8)
  : m_totalX(totalX)
  , m_totalY(totalY)
  , m_totalZ(totalZ)
  , m_bulbSize(bulbSize)
  , m_fBailout(fBailout)
  , m_iMaxIterations(iMaxIterations)
  , m_n(n)  {}
  
  T computePoint(uint64_t sx, uint64_t sy, uint64_t sz) const {
    return computePointFloat(m_bulbSize * double(sx) /
                             (m_totalX - 1) - m_bulbSize / 2.0,
                             m_bulbSize * double(sy) /
                             (m_totalY - 1) - m_bulbSize / 2.0,
                             m_bulbSize * double(sz) /
                             (m_totalZ - 1) - m_bulbSize / 2.0);
  }
  
  double getBailout() const { return m_fBailout; }
  T getMaxIterations() const { return m_iMaxIterations; }
  
private:
  uint64_t m_totalX;
  uint64_t m_totalY;
  uint64_t m_totalZ;
  double m_bulbSize;
  double m_fBailout;
  T m_iMaxIterations;
  uint32_t m_n;
  
  double radius(double x, double y, double z) const {
    return std::sqrt(x * x + y * y + z * z);
  }
  
  double phi(double x, double y) const { return std::atan2(y, x); }
  
  double theta(double x, double y, double z) const {
    return std::atan2(std::sqrt(x * x + y * y), z);
  }
  
  double powerX(double x, double y, double z, double cx, int n,
                double power) const {
    return cx + power * std::sin(theta(x, y, z) * n) * std::cos(phi(x, y) * n);
  }
  
  double powerY(double x, double y, double z, double cy, int n,
                double power) const {
    return cy + power * std::sin(theta(x, y, z) * n) * std::sin(phi(x, y) * n);
  }
  
  double powerZ(double x, double y, double z, double cz, int n,
                double power) const {
    return cz + power * std::cos(theta(x, y, z) * n);
  }
  
  T computePointFloat(double sx, double sy, double sz) const {
    
    double fx = 0;
    double fy = 0;
    double fz = 0;
    double r = 0; // = radius(fx, fy, fz);
    
    for (T i = 0; i < m_iMaxIterations; i++) {
      
      const double fPower = std::pow(r, static_cast<double>(m_n));
      
      const double fx_ = powerX(fx, fy, fz, sx, m_n, fPower);
      const double fy_ = powerY(fx, fy, fz, sy, m_n, fPower);
      const double fz_ = powerZ(fx, fy, fz, sz, m_n, fPower);
      
      fx = fx_;
      fy = fy_;
      fz = fz_;
      
      if ((r = radius(fx, fy, fz)) > m_fBailout)
        return i;
    }
    
    return m_iMaxIterations;
  }
};

/*
 For more information, please see: http://software.sci.utah.edu
 
 The MIT License
 
 Copyright (c) 2012 Interactive Visualization and Data Analysis Group
 
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
