#pragma once

#ifndef UVFBASIC_H
#define UVFBASIC_H

#ifndef UVFVERSION
  #define UVFVERSION 5
#else
  #if UVFVERSION != 5
    #error [UVFBasic.h] Version mismatch
  #endif
#endif

#include "silverbullet/base/StdDefines.h"

#define UVF_INVALID UINT64_INVALID

#include "LargeRAWFile.h"

class DataBlock;

#endif // UVFBASIC_H
