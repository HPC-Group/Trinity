#include "ProgressTimer.h"

#include <sstream>
#include <iomanip>

ProgressTimer::ProgressTimer() : Timer() {}

static std::string TimeToString(double dMiliSecs) {
  const uint64_t miliSecs = uint64_t(dMiliSecs);
  const uint64_t secs   = (miliSecs/(1000))%60;
  const uint64_t mins   = (miliSecs/(1000*60))%60;
  const uint64_t hours  = (miliSecs/(1000*60*60))%24;
  const uint64_t days   = (miliSecs/(1000*60*60*24))%7;
  const uint64_t weeks  = (miliSecs/(1000*60*60*24*7));

  std::stringstream result;

  if (weeks > 0) {
    if (weeks > 1)
      result << weeks << " Weeks ";
    else
      result << weeks << " Week ";
  }

  if (days > 0) {
    if (days > 1)
      result << days << " Days ";
    else
      result << days << " Day ";
  }

  if (hours > 0) {
    result << hours << ":";
  }

  result << std::setw(2) << std::setfill('0') << mins 
         << ":" << std::setw(2) << std::setfill('0') << secs;

  return result.str();
}

std::string ProgressTimer::GetProgressMessage(double progress,
                                              bool bIncludeElapsed,
                                              bool bIncludeRemaining) const {

  const double miliSecs = Elapsed();
  std::string result;

  if (bIncludeElapsed) {
    if (!result.empty()) result += " "; 
    result += "Elapsed: " + TimeToString(miliSecs);
  }
  if (bIncludeRemaining && progress > 0.0) {
    if (!result.empty()) result += " "; 
    result += "Remaining: " + TimeToString(miliSecs * (1.0-progress)/progress);
  }

  return result;
}
