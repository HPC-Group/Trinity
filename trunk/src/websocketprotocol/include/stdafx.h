// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>

#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string.h>

#include <map>
#include <vector>
#include <set>
using namespace std;

// TODO: reference additional headers your program requires here
#include <PushFrameworkInc.h>
using namespace PushFramework;