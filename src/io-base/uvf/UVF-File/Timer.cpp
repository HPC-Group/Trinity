#include <cassert>
#include <cstdlib>
#include "Timer.h"

#ifndef DETECTED_OS_WINDOWS
  using namespace std;
#endif

Timer::Timer() {
#ifndef DETECTED_OS_WINDOWS
#else
  LARGE_INTEGER ticksPerSecond;
  QueryPerformanceFrequency(&ticksPerSecond);
  m_fTicksPerMSecond = double(ticksPerSecond.QuadPart) / 1000.0;
#endif
}

void Timer::Start() {
#ifndef DETECTED_OS_WINDOWS
  gettimeofday(&m_timeStart, NULL);
#else
  QueryPerformanceCounter(&m_timeStart);
#endif

}

double Timer::Elapsed() const {
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

double Timer::Now() {
#ifndef DETECTED_OS_WINDOWS
  struct timeval tm;
  gettimeofday(&tm, NULL);
  return tm.tv_sec + (tm.tv_usec/1000000);
#else
  assert(1 == 0); /// @todo implement
  return -42.4242;
#endif
}
