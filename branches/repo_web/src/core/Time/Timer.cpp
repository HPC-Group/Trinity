#include <cassert>
#include <cstdlib>
#include "Timer.h"

using namespace std;

namespace Core {
  namespace Time {
    
    Timer::Timer() {
#ifdef DETECTED_OS_WINDOWS
      LARGE_INTEGER ticksPerSecond;
      QueryPerformanceFrequency(&ticksPerSecond);
      m_fTicksPerMSecond = double(ticksPerSecond.QuadPart) / 1000.0;
#endif
    }
    
    void Timer::start() {
#ifndef DETECTED_OS_WINDOWS
      gettimeofday(&m_timeStart, NULL);
#else
      QueryPerformanceCounter(&m_timeStart);
#endif
      
    }
    
    double Timer::elapsed() const {
#ifndef DETECTED_OS_WINDOWS
      struct timeval timeProbe;
      gettimeofday(&timeProbe, NULL);
      
      return (timeProbe.tv_sec - m_timeStart.tv_sec) * 1000.0 +
      (timeProbe.tv_usec - m_timeStart.tv_usec) / 1000.0;
#else
      LARGE_INTEGER timeProbe;
      QueryPerformanceCounter(&timeProbe);
      
      ULONGLONG ticksPassed = timeProbe.QuadPart -  m_timeStart.QuadPart;// = tick.QuadPart/ticksPerSecond.QuadPart;
      
      return double(ticksPassed)/m_fTicksPerMSecond;
#endif
    }
    
    double Timer::now() {
#ifndef DETECTED_OS_WINDOWS
      struct timeval tm;
      gettimeofday(&tm, NULL);
      return tm.tv_sec + (tm.tv_usec/1000000);
#else
      assert(1 == 0); /// @todo implement
      return -42.4242;
#endif
    }
  }
}

/*
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
