/********************************************************************
	File :			DemuxImpl_Win.cpp
	Creation date :	2012/01/29

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
#include "..\StdAfx.h"
#include "DemuxImpl_Win.h"

#include "..\Demultiplexor.h"
#include "..\ServerImpl.h"

#ifdef Plateform_Windows
#include "IOQueue_Win.h"
#elif Plateform_Linux
#include "IOQueue_Linux.h"
#endif

#include "..\Dispatcher.h"
#include "..\PhysicalConnection.h"
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
    g_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    g_nThreads = pServerImpl->getWorkerCount();

    g_phWorkerThreads = new HANDLE[g_nThreads];
    DWORD  nThreadID;

    for ( int i = 0; i < g_nThreads; i++ )
    {

        g_phWorkerThreads[i] = CreateThread(NULL,// Security
                               0,						// Stack size - use default
                               KerIOCPWorkerProc,     		// Thread fn entry point
                               (void*) this,			// Param for thread
                               0,						// Init flag
                               &nThreadID);			// Thread address
        if (g_phWorkerThreads[i] == NULL )
        {
            return false;
        }
        m_nWorkerCnt++;
        //CloseHandle(hWorker);
    }
    return true;
}

void DemuxImpl::stop()
{
    //Ask all worker threads to start shutting down
    SetEvent(g_hShutdownEvent);
    for (int i = 0; i < g_nThreads; i++)
    {
        //Help threads get out of blocking - GetQueuedCompletionStatus()
        ioQueue.PostTerminationSignal();
    }
    //Let Worker Threads shutdown
    WaitForMultipleObjects(g_nThreads, g_phWorkerThreads, TRUE, INFINITE);
}
DWORD WINAPI DemuxImpl::KerIOCPWorkerProc( LPVOID WorkContext )
{
    DemuxImpl* pThis = reinterpret_cast<DemuxImpl*>(WorkContext);

    pThis->proc();

    return 0;
}

void DemuxImpl::proc()
{
    void* perThreadCtx = pServerImpl->getFacade()->workerThreadBegin();


    DWORD dwIoSize;
    PhysicalConnection *pPerSocketContext = NULL;
    LPOVERLAPPED lpOverlapped;//Passed to GetCompletionStatus to point to the completed operation's buffer
    OVERLAPPEDPLUS  *pPerIOContext;//OverlapPlus associated to the lpOverlapped returned by GetCompletionStatus(..

    ULONG	ulFlags = MSG_PARTIAL;


    while (WAIT_OBJECT_0 != WaitForSingleObject(g_hShutdownEvent, 0))
    {
		BOOL bSuccess = ioQueue.GetQueuedEvent(&dwIoSize, reinterpret_cast<PULONG_PTR>(&pPerSocketContext), &lpOverlapped);

        //CLoggingFacilities::doLoggingByArgList("IO Completion :%d", dwIoSize);

        if(!bSuccess)
        {
            DWORD dwIOError = GetLastError();

            LPVOID lpMsgBuf;

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dwIOError,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0, NULL );


            if (dwIOError!= WAIT_TIMEOUT )
            {
                if (pPerSocketContext)
                {
                    //Force the removal of this clientEP.
                    //dwIOError==ERROR_NETNAME_DELETED
                    //TRACE(_T("[%x]GetQueuedCompletionStatus failed %x sysMsg:%s\n"),::GetCurrentThreadId(),dwIOError,(LPCTSTR)lpMsgBuf);
                    //pThis->disconnectEndPoint(pPerSocketContext,false,false);					//REMOVED
                }
            }
            continue;
        }
        if (pPerSocketContext==NULL)
        {
            // StopWorkers used PostQueuedCompletionStatus to post an I/O packet with
            // a NULL CompletionKey (or if we get one for any reason).  It is time to exit.
            break;
        }

        pPerIOContext = CONTAINING_RECORD(lpOverlapped, OVERLAPPEDPLUS, m_ol);
        switch (pPerIOContext->m_ioType)
        {
        case IORead:
            dispatcher.OnReceiveComplete(pPerSocketContext,dwIoSize);
            break;
        case IOWrite:
            dispatcher.OnWriteComplete(pPerSocketContext,dwIoSize);
            break;
        case IOInitialize:
            dispatcher.OnInitializeReady(pPerSocketContext);
            delete pPerIOContext;
            break;
        default:
            break;
        }//end of switch

    }//end of while
    pServerImpl->getFacade()->workerThreadEnd(perThreadCtx);
}

}