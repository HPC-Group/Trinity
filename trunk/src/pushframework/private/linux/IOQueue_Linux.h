/*
 * IOCPQueue.h
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#ifndef IOCPQUEUE_H_
#define IOCPQUEUE_H_

namespace PushFramework
{

class IOQueue
{
public:
    IOQueue();
    ~IOQueue(void);
    bool Create();
    void Free();
    void PostTerminationSignal();
    int GetQueuedEvent(epoll_event *pEvents, int nMaxEvents, bool isInputEvents);
    bool AddSocketContext(SOCKET socket,void* lpObject);
    void DeleteSocketContext(SOCKET socket);
    //
    bool RearmSocketInputEvents(SOCKET socket,void* lpObject);
    bool RearmSocketOutputEvents(SOCKET socket,void* lpObject);
private:
    int inPollQueue;
    int outPollQueue;

};
extern IOQueue ioQueue;
}
#endif /* IOCPQUEUE_H_ */
