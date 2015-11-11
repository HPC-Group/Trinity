

#include "../include/Plateform.h"


#ifdef Plateform_Windows
typedef DWORD ThreadIDType;
typedef HANDLE ThreadHandleType;
#define AtomicIncrement(i) ::InterlockedIncrement ( (long*)&i )
#define AtomicDecrement(i) ::InterlockedDecrement ( (long*)&i )
#define AtomicExchange(i, val) InterlockedExchange((long*)&i, val )
#else

#ifdef Plateform_Linux
typedef pthread_t ThreadIDType;
typedef pthread_t ThreadHandleType;
#define AtomicIncrement(i) __sync_add_and_fetch(&i, 1)
#define AtomicDecrement(i) __sync_add_and_fetch(&i, -1)
#define AtomicExchange(i, val) __sync_lock_test_and_set (&i, val)
#endif

#endif


#ifdef Plateform_Linux
#ifndef SOCKET
typedef int SOCKET;
#endif

#ifndef SOCKADDR_IN
typedef sockaddr_in SOCKADDR_IN;
#endif

#ifndef IN_ADDR
typedef in_addr IN_ADDR;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#define Sleep usleep
#endif

