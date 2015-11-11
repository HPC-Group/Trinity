/*
 * DemuxImpl.h
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#ifndef DEMUXIMPL_H_
#define DEMUXIMPL_H_


namespace PushFramework
{
class Demultiplexor;

class DemuxImpl
{
public:
    DemuxImpl(Demultiplexor* pFacade);
    virtual ~DemuxImpl();
    bool start();
    void stop();
private:
    Demultiplexor* pFacade;
private:
    vector<pthread_t> workersThreadsVect;
    bool stopWorking;

    static void*  threadProcForReadEvents (void* WorkContext);
    static void*  threadProcForWriteEvents (void* WorkContext);
    void proc(bool processReadEvents = true);
};

}
#endif /* DEMUXIMPL_H_ */
