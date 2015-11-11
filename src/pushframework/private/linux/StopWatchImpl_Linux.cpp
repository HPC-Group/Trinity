/*
 * StopWatchImpl.cpp
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */
#include "StdAfx.h"
#include "StopWatchImpl_Linux.h"


namespace PushFramework
{



StopWatchImpl::~StopWatchImpl()
{
    // TODO Auto-generated destructor stub
}

StopWatchImpl::StopWatchImpl()
{
    gettimeofday(&tvBegin, NULL);
}

void StopWatchImpl::reset()
{

}

double StopWatchImpl::GetElapsedTime( bool bStart /*= true*/ )
{
    timeval tvEnd, tvDiff;
    gettimeofday(&tvEnd, NULL);

    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);


    double sec = tvDiff.tv_sec;
    double usec = tvDiff.tv_usec;

    return sec + 1000000 / usec;
}

int StopWatchImpl::timeval_subtract( struct timeval *result, struct timeval *t2, struct timeval *t1 )
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff < 0);
}
}
