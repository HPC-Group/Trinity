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
  \file    NSLogOut.cpp
  \author  Alexander Schiewe
           Intel Visual Computing Institute
           Saarland University
  \date    June 2012
*/

#include <cstring>
#include "NSLogOut.h"
//#import <Foundation/Foundation.h>
#import "ColorLog.h"

using namespace IVDA;

NSLogOut::NSLogOut() {
  Message("NSLogOut::NSLogOut:","Starting up NSLogDebug out");
}

NSLogOut::~NSLogOut() {
  Message("NSLogOut::~NSLogOut:","Shutting down NSLogDebug out");
}

void NSLogOut::printf(enum DebugChannel channel, const char* source, const char* msg)
{
  @autoreleasepool {
    char buff[16384];
    strncpy(buff, msg, 16384);
    switch(channel) {
      case CHANNEL_FINAL:   /* FALL THROUGH */
      case CHANNEL_NONE:    NSLog(@"\t%s (%s): %s\n", ChannelToString(channel), source, buff); break;
      case CHANNEL_ERROR:   NSLogColor(LCL_RED    @"\t%s" LCL_RESET " (%s): %s\n", ChannelToString(channel), source, buff); break;
      case CHANNEL_WARNING: NSLogColor(LCL_YELLOW @"\t%s" LCL_RESET " (%s): %s\n", ChannelToString(channel), source, buff); break;
      case CHANNEL_MESSAGE: NSLogColor(LCL_WHITE  @"\t%s" LCL_RESET " (%s): %s\n", ChannelToString(channel), source, buff); break;
      case CHANNEL_OTHER:   NSLogColor(LCL_BLUE   @"\t%s" LCL_RESET " (%s): %s\n", ChannelToString(channel), source, buff); break;
    }
  }
}

void NSLogOut::printf(const char *s) const
{
  @autoreleasepool {
    NSLog(@"%s", s);
  }
}

bool NSLogOut::IsXcodeColorsPluginEnabled() const
{
  return IsXcodeColorsEnabled();
}
