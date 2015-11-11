/*
 * StopWatchImpl.h
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#ifndef STOPWATCHIMPL_H_
#define STOPWATCHIMPL_H_


namespace PushFramework
{
class StopWatchImpl
{
public:
    virtual ~StopWatchImpl();


    StopWatchImpl();

    void reset();
    double GetElapsedTime(bool bStart = true);
private:
    timeval tvBegin, tvLast;

    int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
};

}
#endif /* STOPWATCHIMPL_H_ */
