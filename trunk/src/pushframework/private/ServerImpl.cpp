/********************************************************************
	File :			ServerImpl.cpp
	Creation date :	2010/6/27

	License :			Copyright 2010 Ahmed Charfeddine, http://www.pushframework.com

				   Licensed under the Apache License, Version 2.0 (the "License");
				   you may not use this file except in compliance with the License.
				   You may obtain a copy of the License at

					   http://www.apache.org/licenses/LICENSE-2.0

				   Unless required by applicable law or agreed to in writing, software
				   distributed under the License is distributed on an "AS IS" BASIS,
				   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
				   See the License for the specific language governing permissions and
				   limitations under the License.


*********************************************************************/
#include "StdAfx.h"
#include "ServerImpl.h"

#include "Demultiplexor.h"
#include "Dispatcher.h"
#include "ServerStats.h"

#ifdef Plateform_Windows
#include "win\IOQueue_Win.h"
#else
#ifdef Plateform_Linux
#include "linux\IOQueue_Linux.h"
#endif
#endif

#include "Acceptor.h"
#include "MonitorAcceptor.h"
#include "../include/BroadcastManager.h"
#include "ChannelFactory.h"
#include "../include/ClientFactory.h"
#include "ClientFactoryImpl.h"
#include "BroadcastManagerImpl.h"
#include "../include/Server.h"
#include "MonitorsBroadcastManager.h"
#include "GarbageCollector.h"
#include "BroadcastStreamerManager.h"

#ifdef Plateform_Windows
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

#include "Utilities.h"


namespace PushFramework
{

ServerImpl *pServerImpl;

void ServerImpl::setProtocol( Protocol* pProtocol )
{
    this->pProtocol = pProtocol;
}

void ServerImpl::setClientFactory( ClientFactory* pClientFactory )
{
    this->pClientFactory = pClientFactory;
}

void ServerImpl::setWorkerCount( int workerCount )
{
    this->workerCount = workerCount;
}

ClientFactory* ServerImpl::getClientFactory()
{
    return pClientFactory;
}

unsigned int ServerImpl::getSamplingRate()
{
    return samplingRate;
}

std::string ServerImpl::getMonitorPassword()
{
    return password;
}


#ifdef Plateform_Windows
void ServerImpl::eternal_loop()
{
     pFacade->OnStarted();

    //Periodic Timer
    m_hEventTimer = CreateEvent(NULL, TRUE, FALSE, NULL);
    ResetEvent(m_hEventTimer);
    //Abort event. (set by Stop function)
    m_hEventAbort = CreateEvent(NULL, TRUE, FALSE, NULL);
    ResetEvent(m_hEventAbort);
    //

    HANDLE			hWaits[2];
    hWaits[0]		= m_hEventTimer;
    hWaits[1]		= m_hEventAbort;


    UINT			sleepTimeMs=1000;//1 second.

    nSecsGCCount=0;
    nSecsPerfObservCount=0;
    //

    while(true)
    {
        MMRESULT result = timeSetEvent(sleepTimeMs, 1,
                                       (LPTIMECALLBACK) m_hEventTimer, 0, TIME_ONESHOT|TIME_CALLBACK_EVENT_SET);
        _ASSERT(result != NULL);

        // Sleep here until the timer interval ends or abort occurs
        if(WaitForMultipleObjects(2, hWaits, FALSE, INFINITE)==0)
        {

            DoPeriodicJobs();
            ResetEvent(m_hEventTimer);
        }
        else
        {
            //m_hEventAbort is set : break from while.
            break;
        }
        //
    }

    CloseHandle(m_hEventAbort);
    CloseHandle(m_hEventTimer);
    //quit blocking.
}

DWORD WINAPI ServerImpl::threadProc( LPVOID WorkContext )
{
    ServerImpl* pMe = reinterpret_cast<ServerImpl*>(WorkContext);
    pMe->eternal_loop();
    return 0;
}
#endif


#ifdef Plateform_Linux
void ServerImpl::eternal_loop()
{


    pFacade->OnStarted();


    int sec = 1;
    struct timespec ts;
    pthread_mutex_t cs;
    pthread_mutex_init(&cs, NULL);
    pthread_mutex_lock(&cs);
    pthread_cond_init(&abortCond, NULL);

    nSecsGCCount = 0;
    nSecsPerfObservCount = 0;

    while (true)
    {
        ::clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += sec;

        int rc = pthread_cond_timedwait(&abortCond, &cs, &ts);
        if (rc == ETIMEDOUT)
        {

            DoPeriodicJobs();
        }
        else
        {
            //Server is to die.
            break;
        }

    }


    pthread_mutex_unlock(&cs);

}
void* ServerImpl::threadProc(void* WorkContext)
{
    ServerImpl* pMe = reinterpret_cast<ServerImpl*> (WorkContext);
    pMe->eternal_loop();
    return 0;
}
#endif






void ServerImpl::start( bool startInSeparateThread )
{
    if(pClientFactory == NULL)
        throw Exception("Fatal : no ClientFactory instance was set");

    assignDefaultProtocol();

    if (!ioQueue.Create())
        throw Exception("Internal problem : unable to create iocp queue");

    if (!demux.start())
    {
        ioQueue.Free();
        throw Exception("Internal problem : unable to start demultiplexor");
    }

    if (!startListeners())
    {
        stopListeners();
        demux.stop();
        ioQueue.Free();
    }


    if ( isMonitorEnabled && !monitorAcceptor.startListening())
    {
        stopListeners();
        demux.stop();
        ioQueue.Free();
        throw Exception("Internal problem : unable to start monitoring acceptor");
    }

    broadcastStreamerManager.Start();

    if (startInSeparateThread)
    {
#ifdef Plateform_Windows
        DWORD  nThreadID;

        hThread = CreateThread(NULL,// Security
                                         0,						// Stack size - use default
                                         threadProc,     		// Thread fn entry point
                                         (void*) this,			// Param for thread
                                         0,						// Init flag
                                         &nThreadID);			// Thread address
#else
#ifdef Plateform_Linux
        pthread_create(&hThread, NULL, threadProc, (void*) this);
#endif
#endif
    }
    else
        eternal_loop();

}

void ServerImpl::stop()
{
#ifdef Plateform_Windows
    SetEvent(m_hEventAbort);//stops eternal_loop
#else
#ifdef Plateform_Linux
    pthread_cond_signal(&abortCond);
#endif
#endif

    if (isMonitorEnabled)
        monitorAcceptor.stopListening();


    stopListeners();


    demux.stop();

    broadcastManagerImpl.DisposeAllPackets();

    ioQueue.Free();
}

void ServerImpl::registerService( unsigned int serviceId, Service* pService, const char* serviceName )
{
    dispatcher.registerService(serviceId, pService, serviceName);
}

ServerImpl::ServerImpl( Server* pFacade )
{
#ifdef Plateform_Windows
    //Initialize WinSock :
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

    this->pFacade = pFacade;

    pProtocol = NULL;
    pClientFactory = NULL;
    listeningPort = 2010;

    workerCount = Utilities::getProcessorsCount() * 2;


    isMonitorEnabled = false;
    isProfilingEnabled = false;
    monitorPort = 2011;
    samplingRate = 10;
    password = "";

    uGCPeriod = 10;

    pServerImpl = this;
}

ServerImpl::~ServerImpl( void )
{
}

Server* ServerImpl::getFacade()
{
    return pFacade;
}

void ServerImpl::enableRemoteMonitor( short sPort, const char* password )
{
    this->monitorPort = sPort;
    this->password = password;
    isMonitorEnabled = true;
    monitorAcceptor.setListeningPort(monitorPort);
}

void ServerImpl::enableProfiling( unsigned int samplingRate )
{
    if (samplingRate >= 0)
    {
        this->samplingRate = samplingRate;
    }

    isProfilingEnabled = true;
}

void ServerImpl::disableProfiling()
{
    isProfilingEnabled = false;
}

bool ServerImpl::getProfilingStatus()
{
    return isProfilingEnabled;
}

void ServerImpl::setServeInfos( const char* serverInfos )
{
    this->serverInfos = serverInfos;
}

std::string ServerImpl::getServerInfos()
{
    return serverInfos;
}




void ServerImpl::setLoginExpiryDuration( unsigned int uLoginExpireDuration )
{
    channelFactory.setLoginExpiryDuration(uLoginExpireDuration);
}

void ServerImpl::pause()
{
    stopListeners();
}

void ServerImpl::resume()
{
    startListeners();
}

void ServerImpl::getVersion( Version& ver )
{
    ver.major = 1;
    ver.minor = 0;
    ver.monitorProtocolVer = 1;
}

int ServerImpl::getWorkerCount()
{
    return workerCount;
}




bool ServerImpl::createListener( int port, ListenerOptions* pOptions /*= NULL*/ )
{
    listenersMapT::iterator it = listenersMap.find(port);
    if (it != listenersMap.end())
        return false;

    Acceptor* pAcceptor = new Acceptor();
    pAcceptor->setListeningPort(port);

    if (pOptions)
        pAcceptor->getOptions() = *pOptions;

    listenersMap[port] =  pAcceptor;
    return true;
}




bool ServerImpl::startListeners( )
{
    for (listenersMapT::iterator it = listenersMap.begin();
            it!=listenersMap.end();
            it++)
    {
        Acceptor* pAcceptor = it->second;
        //
        //Start it :
        if (!pAcceptor->startListening())
        {
            return false;

        }
    }
    return true;
}

void ServerImpl::assignDefaultProtocol()
{
    for (listenersMapT::iterator it = listenersMap.begin();
            it!=listenersMap.end();
            it++)
    {
        Acceptor* pAcceptor = it->second;
        //
        if(pAcceptor->getOptions().pProtocol == NULL)
        {
            if(pProtocol == NULL)
            {
                std::string strError = Utilities::stringBuilderA("FATAL : no protocol instance for acceptor on port : %d", it->first);
                throw Exception(strError.c_str());
            }
            pAcceptor->getOptions().pProtocol = pProtocol;
        }
    }
}

void ServerImpl::stopListeners()
{
    for (listenersMapT::iterator it = listenersMap.begin();
            it!=listenersMap.end();
            it++)
    {
        Acceptor* pAcceptor = it->second;
        pAcceptor->stopListening();
    }
}

void ServerImpl::setMaxConnections( int nMaxConnections )
{
    channelFactory.setMaxAllowedConnections(nMaxConnections);
}

void ServerImpl::EncodeOnAllProtocols( OutgoingPacket& outgoingPacket )
{
    for (listenersMapT::iterator it = listenersMap.begin();
            it!=listenersMap.end();
            it++)
    {
        Protocol* pProtocol = it->second->getOptions().pProtocol;
        pProtocol->encodeOutgoingPacket(outgoingPacket);
    }
}

void ServerImpl::DoPeriodicJobs()
{
    nSecsGCCount++;
    nSecsPerfObservCount++;

    if(nSecsGCCount==uGCPeriod)
    {
        nSecsGCCount=0;
        garbageCollector.Activate();
        channelFactory.CloseNonLogged();
    }

    if(nSecsPerfObservCount==samplingRate && isProfilingEnabled)
    {
        nSecsPerfObservCount=0;
        OutgoingPacket* pPacket = stats.getPerformancePacket();
        monitorBroadcastManager.PushPacket(pPacket, "stats");
    }
}



}

