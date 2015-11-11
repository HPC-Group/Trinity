#include "StdAfx.h"
#include "StopWatch.h"

#ifdef Plateform_Windows
#include "win\StopWatchImpl_Win.h"
#else
#ifdef Plateform_Linux
#include "linux\StopWatchImpl_Linux.h"
#endif
#endif


namespace PushFramework
{

StopWatch::StopWatch()
{
    pImpl = new StopWatchImpl;
}

StopWatch::~StopWatch(  )
{

}

void StopWatch::reset()
{
    pImpl->reset();
}

double StopWatch::GetElapsedTime( bool bStart /*= true*/ )
{
    return pImpl->GetElapsedTime();
}
}
