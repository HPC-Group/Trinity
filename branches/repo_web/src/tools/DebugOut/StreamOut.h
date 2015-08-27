#pragma once

#ifndef IVDA_STREAMOUT_H
#define IVDA_STREAMOUT_H

#include "AbstrDebugOut.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <set>

namespace IVDA
{
  class StreamOut : public AbstrDebugOut
  {
  public:
    StreamOut(std::vector<std::string> const& vSourceBlacklist = std::vector<std::string>(), bool bOmitChannel = true, bool bOmitSource = true);
    ~StreamOut();
    virtual void printf(enum DebugChannel, const char* source, const char* msg);
    virtual void printf(const char *s) const;

  private:
    bool m_bOmitChannel;
    bool m_bOmitSource;
    std::unordered_set<std::string> m_SourceBlacklist;
    std::set<size_t> m_SourceBlacklistChars;
  };

}; // namespace IVDA

#endif // IVDA_STREAMOUT_H
