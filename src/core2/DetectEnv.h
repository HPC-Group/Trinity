#ifndef DETECTENV_H
#define DETECTENV_H

// undef all OS types first
#ifdef DETECTED_OS_WINDOWS
#undef DETECTED_OS_WINDOWS
#elif defined(DETECTED_OS_APPLE)
#undef DETECTED_OS_APPLE
#ifdef DETECTED_IOS
#undef DETECTED_IOS
#elif defined(DETECTED_IOS_SIMULATOR)
#undef DETECTED_IOS_SIMULATOR
#elif defined(DETECTED_OS_MAC)
#undef DETECTED_OS_MAC
#endif
#elif defined(DETECTED_OS_LINUX)
#undef DETECTED_OS_LINUX
#endif

// now figure out which OS we are compiling on
#ifdef _WIN32
#define DETECTED_OS_WINDOWS
#elif defined(macintosh) || (defined(__MACH__) && defined(__APPLE__))
#define DETECTED_OS_APPLE
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR > 0
#define DETECTED_IOS_SIMULATOR
#elif TARGET_OS_IPHONE > 0
#define DETECTED_IOS
#elif TARGET_OS_MAC > 0
#define DETECTED_OS_MAC
#endif
#elif defined(__linux__)
#define DETECTED_OS_LINUX
#endif

#endif // DETECTENV_H

/*
 The MIT License
 
 Copyright (c) 2014 HPC Group, Univeristy Duisburg-Essen
 
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
