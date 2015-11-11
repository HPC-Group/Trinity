/*
 * ListenerImpl.cpp
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#include "StdAfx.h"
#include "ListenerImpl_Linux.h"

#include "Listener.h"

namespace PushFramework
{
ListenerImpl::ListenerImpl(Listener* pFacade)
{
    // TODO Auto-generated constructor stub
    this->pFacade = pFacade;

}

ListenerImpl::~ListenerImpl()
{
    // TODO Auto-generated destructor stub
}

bool ListenerImpl::startListening()
{
    //Create the socket :
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket < 0)
    {
        cout << "unable to create listening socket" << endl;
        return false;
    }


    int on = 1;
    if (setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on)))
    {
        cout << "Could not set socket %d option for reusability" << endl;
        close(hSocket);
        return false;
    }



    //Prepare address to bind to :
    SOCKADDR_IN saServer;
    saServer.sin_port = htons(pFacade->uPort);
    saServer.sin_family = AF_INET;
    if (pFacade->options.interfaceAddress != NULL)
    {
        u_long nInterfaceAddr = inet_addr(pFacade->options.interfaceAddress);
        if (nInterfaceAddr != INADDR_NONE)
        {
            close(hSocket);
            return false;
        }
        saServer.sin_addr.s_addr = nInterfaceAddr;
    }
    else
        saServer.sin_addr.s_addr = INADDR_ANY;


    //Bind the socket :
    int nRet = bind(hSocket, (struct sockaddr *) &saServer, sizeof (struct sockaddr));
    if (nRet < 0)
    {
        cout << "Could not bind socket to address" << endl;
        close(hSocket);
        return false;
    }


    //Set socket to listening mode :
    if (listen(hSocket, pFacade->options.listeningBackLog))
    {
        cout << "unable to listen" << endl;
        close(hSocket);
        return false;
    }

    //Create kill event used by ::stop to terminate the listening thread
    stopListen = false;

    int iret1 = pthread_create(&hThread, NULL, threadProc, (void*) this);

    return true;
}

void ListenerImpl::stopListening()
{
    stopListen = true;
    shutdown(hSocket, SHUT_RDWR);
    pthread_join(hThread, NULL);
}

void* ListenerImpl::threadProc(void* lpVoid)
{
    ListenerImpl* pThis = reinterpret_cast<ListenerImpl*> (lpVoid);
    pThis->doListen();
    return 0;
}

bool ListenerImpl::acceptConnectionRequest(SOCKET clientSocket, SOCKADDR_IN SockAddr)
{

    //Set as non-blocking :
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK))
    {
        cout << "Could not make the socket non-blocking" << endl;
        shutdown(clientSocket, 2);
        close(clientSocket);
        return true;
    }

    //Start servicing new connection
    if (!pFacade->handleAcceptedSocket(clientSocket, SockAddr))
    {
        shutdown(clientSocket, 2);
        close(clientSocket);
        return true;
    }
    return true;
}

void ListenerImpl::doListen()
{

    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof (cli_addr);

    while (!stopListen)
    {
        int clientSocket = accept(hSocket,
                                  (struct sockaddr *) &cli_addr, &clilen);

        if (clientSocket < 0)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
            perror("Error on accept : ");
            //cout << "Error on accept : " << errno << endl;
            break;
            //if(errno != EINTR && errno != EAGAIN)
        }
        acceptConnectionRequest(clientSocket, cli_addr);
    }
    close(hSocket);
}


}


