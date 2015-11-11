/*
 * DemuxImpl.cpp
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */
#include "StdAfx.h"
#include "DemuxImpl_Linux.h"

#include "Demultiplexor.h"
#include "ServerImpl.h"

#ifdef Plateform_Windows
#include "IOQueue_Win.h"
#else
#ifdef Plateform_Linux
#include "IOQueue_Linux.h"
#endif
#endif

#include "Dispatcher.h"
#include "PhysicalConnection.h"
#include "../include/Server.h"


namespace PushFramework
{
DemuxImpl::DemuxImpl(Demultiplexor* pFacade)
{
    // TODO Auto-generated constructor stub
    this->pFacade = pFacade;
}

DemuxImpl::~DemuxImpl()
{
    // TODO Auto-generated destructor stub
}

bool DemuxImpl::start()
{
    stopWorking = false;

    int nThreads = pServerImpl->getWorkerCount();
    for (int i = 0; i < nThreads; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, threadProcForReadEvents, (void*) this);
        workersThreadsVect.push_back(thread);
    }
    for (int i = 0; i < nThreads; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, threadProcForWriteEvents, (void*) this);
        workersThreadsVect.push_back(thread);
    }
    return true;

}

void DemuxImpl::stop()
{
    stopWorking = true;
    for (int i = 0; i < workersThreadsVect.size(); i++)
    {
        pthread_t hThread = workersThreadsVect.at(i);
        pthread_join(hThread, NULL);
    }
}

void* DemuxImpl::threadProcForReadEvents(void* WorkContext)
{
    DemuxImpl* pThis = reinterpret_cast<DemuxImpl*> (WorkContext);

    pThis->proc();

    return 0;
}
void* DemuxImpl::threadProcForWriteEvents(void* WorkContext)
{
    DemuxImpl* pThis = reinterpret_cast<DemuxImpl*> (WorkContext);

    pThis->proc(false);

    return 0;
}

void DemuxImpl::proc(bool processReadEvents)
{
    void* perThreadCtx = pServerImpl->getFacade()->workerThreadBegin();

    int epollArraySize = 64;
    struct epoll_event epoll_events[64];

    while (!stopWorking)
    {
        int rval = ioQueue.GetQueuedEvent(epoll_events, epollArraySize, processReadEvents);

        if (rval == 0)
        {
            //cout << "timeout happened" << endl;
            continue;
        }

        if (rval < 0)
        {
            cout << "FATAL" << endl;
            return;
        }

        //Start handling events :
        for (int i = 0; i < rval; i++)
        {
            //
            uint32_t events = epoll_events[i].events;
            int fd = epoll_events[i].data.fd;
            PhysicalConnection* pPerSocketContext = (PhysicalConnection*) (epoll_events[i].data.ptr);

            if (events & EPOLLERR)
            {
                shutdown(fd, SHUT_RDWR);
                close(fd);
                continue;
            }
            if (events & EPOLLHUP)
            {
                shutdown(fd, SHUT_RDWR);
                close(fd);
                continue;
            }
            if (events & EPOLLRDHUP)
            {
                shutdown(fd, SHUT_RDWR);
                close(fd);
                continue;
            }
            //
            if (events & EPOLLOUT)
            {
                dispatcher.OnWriteComplete(pPerSocketContext, -1);
            }
            if (events & EPOLLIN)
            {
                dispatcher.OnReceiveComplete(pPerSocketContext, -1);
            }
        }
    }
    pServerImpl->getFacade()->workerThreadEnd(perThreadCtx);
}

}

