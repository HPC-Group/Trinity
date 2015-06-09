#include "StreamOut.h"
#include <iostream>

using namespace IVDA;

#ifdef DETECTED_OS_WINDOWS
#include <windows.h>

#define IVDA_FOREGROUND_BLACK     0x0000
#define IVDA_FOREGROUND_BLUE      0x0001
#define IVDA_FOREGROUND_GREEN     0x0002
#define IVDA_FOREGROUND_CYAN      0x0003
#define IVDA_FOREGROUND_RED       0x0004
#define IVDA_FOREGROUND_MAGENTA   0x0005
#define IVDA_FOREGROUND_YELLOW    0x0006
#define IVDA_FOREGROUND_GREY      0x0007
#define IVDA_FOREGROUND_INTENSITY 0x0008 // foreground color is intensified.

#define IVDA_BACKGROUND_BLACK     0x0000
#define IVDA_BACKGROUND_BLUE      0x0010
#define IVDA_BACKGROUND_GREEN     0x0020
#define IVDA_BACKGROUND_CYAN      0x0030
#define IVDA_BACKGROUND_RED       0x0040
#define IVDA_BACKGROUND_MAGENTA   0x0050
#define IVDA_BACKGROUND_YELLOW    0x0060
#define IVDA_BACKGROUND_GREY      0x0070
#define IVDA_BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#endif

StreamOut::StreamOut(std::vector<std::string> const& vSourceBlacklist, bool bOmitChannel, bool bOmitSource)
  : AbstrDebugOut()
  , m_bOmitChannel(bOmitChannel)
  , m_bOmitSource(bOmitSource)
  , m_SourceBlacklist(vSourceBlacklist.cbegin(), vSourceBlacklist.cend())
  , m_SourceBlacklistChars()
{
  for (auto s = vSourceBlacklist.cbegin(); s != vSourceBlacklist.cend(); ++s)
    m_SourceBlacklistChars.insert(s->length());

  Message("StreamOut::StreamOut:","Starting up StreamDebug out");
}

StreamOut::~StreamOut()
{
  Message("StreamOut::~StreamOut:","Shutting down StreamDebug out");
}

void StreamOut::printf(enum DebugChannel channel, const char* source, const char* msg)
{
  // check black listed sources
  std::string const strSource(source);
  for (auto l = m_SourceBlacklistChars.cbegin(); l != m_SourceBlacklistChars.cend(); ++l) {
    if (m_SourceBlacklist.find(strSource.substr(0, *l)) != m_SourceBlacklist.cend())
      return; // found black listed source
  }

#ifdef DETECTED_OS_WINDOWS
  // remember how things were when we started
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiOut;
  CONSOLE_SCREEN_BUFFER_INFO csbiErr;
  GetConsoleScreenBufferInfo(hOut, &csbiOut);
  GetConsoleScreenBufferInfo(hErr, &csbiErr);
  switch(channel) {
    case CHANNEL_FINAL:   /* FALL THROUGH */
    case CHANNEL_NONE:    SetConsoleTextAttribute(hOut, csbiOut.wAttributes); break;
    case CHANNEL_ERROR:   SetConsoleTextAttribute(hErr, IVDA_FOREGROUND_INTENSITY | IVDA_FOREGROUND_RED); break;
    case CHANNEL_WARNING: SetConsoleTextAttribute(hOut, IVDA_FOREGROUND_INTENSITY | IVDA_FOREGROUND_YELLOW); break;
    case CHANNEL_MESSAGE: SetConsoleTextAttribute(hOut, csbiOut.wAttributes); break;
    case CHANNEL_OTHER:   SetConsoleTextAttribute(hOut, csbiOut.wAttributes); break;
  }
#endif

  std::ostream* pStream = nullptr;
  switch(channel) {
    case CHANNEL_ERROR:
      pStream = &std::cerr; break;
    default:
      pStream = &std::cout; break;
  }

  if (!m_bOmitChannel)
    *pStream << ChannelToString(channel) << " ";
  if (!m_bOmitSource)
    *pStream << "(" << source << "): ";

  *pStream << msg << std::endl;
  pStream->flush();

#ifdef DETECTED_OS_WINDOWS
  // reset how things were when we started
  SetConsoleTextAttribute(hOut, csbiOut.wAttributes);
  SetConsoleTextAttribute(hErr, csbiErr.wAttributes);
#endif
}

void StreamOut::printf(const char *s) const
{
  std::cout << s << std::endl;
  std::cout.flush();
}
