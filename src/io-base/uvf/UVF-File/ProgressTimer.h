#pragma once

#include "Timer.h"
#include <string>

/** \class ProgressTimer */
class ProgressTimer : public Timer {
public:
  ProgressTimer();
  std::string GetProgressMessage(double progress, bool bIncludeElapsed=true,
                                 bool bIncludeRemaining=true) const;
};
