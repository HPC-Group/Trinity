#pragma once

#include "silverbullet/base/StdDefines.h"

#ifndef DETECTED_OS_WINDOWS
  #include <sys/time.h>
#else
  #include <windows.h>
#endif

/** \class Timer */
class Timer {
public:
  Timer();
  void Start();
  double Elapsed() const; // returns milliseconds
  static double Now();
private:
  #ifndef DETECTED_OS_WINDOWS
    timeval m_timeStart;
  #else
    double m_fTicksPerMSecond;
    LARGE_INTEGER m_timeStart;
  #endif
};
