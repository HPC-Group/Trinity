/*
 * ListenerImpl.h
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#ifndef LISTENERIMPL_H_
#define LISTENERIMPL_H_



#include "Types.h"


namespace PushFramework
{

class Listener;

class ListenerImpl
{
public:
    ListenerImpl(Listener* pFacade);
    virtual ~ListenerImpl();
    bool startListening();
    void stopListening();
private:
    Listener* pFacade;
private:
    SOCKET					hSocket; //socket handle.
    pthread_t				hThread;//Handle to the listening thread.
    static void* threadProc(void* lpVoid);
    void doListen();
    bool acceptConnectionRequest(SOCKET clientSocket, SOCKADDR_IN SockAddr);
    bool stopListen;

};


}
#endif /* LISTENERIMPL_H_ */
