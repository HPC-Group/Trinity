#pragma once

#ifndef IVDA_LOG_H
#define IVDA_LOG_H

#include "Singleton.h"
#include "DebugOut/MultiplexOut.h"
#include "DebugOut/ConsoleOut.h"
#include <sstream>

namespace IVDA
{
  class DebugOutHandler
  {
    SINGLETON_IVDA_HEAP(DebugOutHandler);
  public:

    /// Add another debug output
    /// @param debugOut      the new stream (will be deleted on RemoveDebugOut)
    void AddDebugOut(AbstrDebugOut* debugOut);

    /// Removes the given debug output stream.
    /// The stream must be the currently connected/used one.
    void RemoveDebugOut(AbstrDebugOut* debugOut);

    /// Access the currently-active debug stream.
    AbstrDebugOut* DebugOut();
    const AbstrDebugOut *DebugOut() const;

  private:
    MultiplexOut m_DebugOut;
    ConsoleOut m_DefaultOut;
  };

} // namespace IVDA

#ifdef DETECTED_OS_WINDOWS
#pragma warning(disable:4127) // disable conditional expression is constant warning
#endif

#define IVDA_ERRORV(...)                                                                        \
  do {                                                                                          \
    IVDA::DebugOutHandler::Instance().DebugOut()->Error(_IVDA_func_, __VA_ARGS__);              \
  } while(0)
#define IVDA_WARNINGV(...)                                                                      \
  do {                                                                                          \
    IVDA::DebugOutHandler::Instance().DebugOut()->Warning(_IVDA_func_, __VA_ARGS__);            \
  } while(0)
#define IVDA_MESSAGEV(...)                                                                      \
  do {                                                                                          \
    IVDA::DebugOutHandler::Instance().DebugOut()->Message(_IVDA_func_, __VA_ARGS__);            \
  } while(0)
#define IVDA_OTHERV(...)                                                                        \
  do {                                                                                          \
    IVDA::DebugOutHandler::Instance().DebugOut()->Other(_IVDA_func_, __VA_ARGS__);              \
  } while(0)

#define IVDA_ERROR(ostream)                                                                     \
  do {                                                                                          \
    std::stringstream ss;                                                                       \
    ss << ostream;                                                                              \
    IVDA::DebugOutHandler::Instance().DebugOut()->Error(_IVDA_func_, "%s", ss.str().c_str());   \
  } while(0)
#define IVDA_WARNING(ostream)                                                                   \
  do {                                                                                          \
    std::stringstream ss;                                                                       \
    ss << ostream;                                                                              \
    IVDA::DebugOutHandler::Instance().DebugOut()->Warning(_IVDA_func_, "%s", ss.str().c_str()); \
  } while(0)
#define IVDA_MESSAGE(ostream)                                                                   \
  do {                                                                                          \
    std::stringstream ss;                                                                       \
    ss << ostream;                                                                              \
    IVDA::DebugOutHandler::Instance().DebugOut()->Message(_IVDA_func_, "%s", ss.str().c_str()); \
  } while(0)
#define IVDA_OTHER(ostream)                                                                     \
  do {                                                                                          \
    std::stringstream ss;                                                                       \
    ss << ostream;                                                                              \
    IVDA::DebugOutHandler::Instance().DebugOut()->Other(_IVDA_func_, "%s", ss.str().c_str());   \
  } while(0)

#define T_ERROR(...)																	 \
do {																					 \
	IVDA::DebugOutHandler::Instance().DebugOut()->Error(_IVDA_func_, __VA_ARGS__);       \
} while (0)
#define WARNING(...)																			\
do {																							\
	IVDA::DebugOutHandler::Instance().DebugOut()->Warning(_IVDA_func_, __VA_ARGS__);            \
} while (0)
#define MESSAGE(...)																			\
do {																							\
	IVDA::DebugOutHandler::Instance().DebugOut()->Message(_IVDA_func_, __VA_ARGS__);            \
} while (0)
#define OTHER(...)																				\
do {																							\
	IVDA::DebugOutHandler::Instance().DebugOut()->Other(_IVDA_func_, __VA_ARGS__);				\
} while (0)



#ifdef DETECTED_OS_WINDOWS
// ATTENTION: unfortunately we cannot default this compiler warning here because macros would be expanded in code without this pragma
//#pragma warning(default:4127) // restore conditional expression is constant warning
#endif

#endif // IVDA_LOG_H
