/*
 * IOCPQueue.cpp
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#include "StdAfx.h"
#include "IOQueue_Linux.h"


namespace PushFramework
{

IOQueue ioQueue;

IOQueue::IOQueue()
{
}

IOQueue::~IOQueue(void)
{
}

bool IOQueue::Create()
{

    int pollsize = 1024;
    inPollQueue = epoll_create(pollsize);

    if (inPollQueue < 0)
    {
        cout << "unable to create pollQueue" << endl;
        return false;
    }

    outPollQueue = epoll_create(pollsize);

    if (outPollQueue < 0)
    {
        cout << "unable to create pollQueue" << endl;
        close(inPollQueue);
        return false;
    }


    return true;
}

void IOQueue::Free()
{
    close(inPollQueue);
    close(outPollQueue);
}

void IOQueue::PostTerminationSignal()
{
    //Nothing..
}


int IOQueue::GetQueuedEvent(epoll_event *pEvents, int nMaxEvents, bool isInputEvents)
{

    int pollQueue = isInputEvents? inPollQueue : outPollQueue;

    while (true)
    {
        int rval = epoll_wait(pollQueue, pEvents, nMaxEvents, 1000);

        if (rval == 0)
        {
            //cout << "timeout happened" << endl;
            return rval;
        }

        if (rval < 0)
        {
            if (errno == EAGAIN || errno == EINTR)
            {
                continue;
            }
            perror("Error on epoll_wait");
            return rval;
        }

        return rval;
    }
}

bool IOQueue::AddSocketContext(SOCKET clientSocket, void* lpObject)
{

    epoll_event ev;
    ev.events = EPOLLIN |  EPOLLONESHOT /*EPOLLOUT |EPOLLET*/;
    ev.data.ptr = lpObject;

    if (epoll_ctl(inPollQueue, EPOLL_CTL_ADD, clientSocket, &ev) < 0)
    {

        return false;
    }

    ev.events = EPOLLOUT |  EPOLLONESHOT /*EPOLLOUT |EPOLLET*/;
    ev.data.ptr = lpObject;

    if (epoll_ctl(outPollQueue, EPOLL_CTL_ADD, clientSocket, &ev) < 0)
    {
        epoll_ctl(inPollQueue,EPOLL_CTL_DEL,clientSocket, NULL);
        return false;
    }


    return true;
}

void IOQueue::DeleteSocketContext(SOCKET socket)
{
    epoll_ctl(inPollQueue,EPOLL_CTL_DEL,socket, NULL);
    epoll_ctl(outPollQueue,EPOLL_CTL_DEL,socket, NULL);
}

bool IOQueue::RearmSocketInputEvents(SOCKET clientSocket, void* lpObject)
{

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.ptr = lpObject;

    if (epoll_ctl(inPollQueue, EPOLL_CTL_MOD, clientSocket, &ev) < 0)
    {

        perror("Unable to re-arm socket");
        return false;
    }
    return true;

}

bool IOQueue::RearmSocketOutputEvents(SOCKET clientSocket, void* lpObject)
{

    epoll_event ev;
    ev.events = EPOLLOUT| EPOLLONESHOT;
    ev.data.ptr = lpObject;

    if (epoll_ctl(outPollQueue, EPOLL_CTL_MOD, clientSocket, &ev) < 0)
    {

        perror("Unable to re-arm socket");
        return false;
    }
    return true;
}


}

