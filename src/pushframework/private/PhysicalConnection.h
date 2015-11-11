/********************************************************************
	File :			PhysicalConnection.h
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
#ifndef PhysicalConnection__INCLUDED
#define PhysicalConnection__INCLUDED

#pragma once

#include "../include/DataBuffer.h"
#include "../include/Protocol.h"
#include "BroadcastContext.h"

#include "Overlap.h"

namespace PushFramework
{



class ServerImpl;
struct ListenerOptions;
class LogicalConnection;
class GarbageCollector;
class ConnectionContext;

class PhysicalConnection
{
    friend class GarbageCollector;
public:
    enum
    {
        Disposable,
        WaitingForWrite,
        Connected,
        Attached,
    };
    PhysicalConnection(SOCKET s, SOCKADDR_IN address, bool bIsObserver, ListenerOptions* pListenerOption);
    ~PhysicalConnection();


    int getStatus();
    Protocol* getProtocol();
    bool isObserverChannel();
    const char* getLogicalConnectionKey();
    LogicalConnection* getLogicalConnection();

    bool CheckIfUnusedByIOWorkers();
    int getIoWorkersReferenceCounter() const;
    void incrementIoWorkersReferenceCounter();
    void decrementIoWorkersReferenceCounter();

    UINT getPeerPort();
    std::string getPeerIP();

    double getLifeDuration();

	void SetBinaryFlag(bool binary);

private:
	bool isBinary = false;
    bool bIsObserver;
    ListenerOptions* pListenerOption;
    int status;//status
    int ioWorkersReferenceCounter;
    time_t dtCreationTime;
    CRITICAL_SECTION csLock;
    LogicalConnection* pLogicalConnection;
    std::string clientKey;//For observers only.
public:
    void Close(bool bWaitForSendsToComplete);
    void PostReceive();
    int ReadReceivedBytes(int& dwIoSize);
    SendResult PushPacket(OutgoingPacket* pPacket);

    SendResult PushPacketCommon( OutgoingPacket* pPacket );

    SendResult TryPushPacket(OutgoingPacket* pPacket);
    int OnSendCompleted(int dwIoSize, bool& bIsBufferIdle);
    DataBuffer& GetReceiveBuffer();
private:
    DataBuffer inBuffer;

#ifdef Plateform_Windows
    //operation read buffer
    WSABUF	m_wsaInBuffer;
    BYTE *m_byInBuffer;//buffer passed for each read op
    OVERLAPPEDPLUS *pReadOverlap;

    WSABUF m_wsaOutBuffer;
    BYTE *m_byOutBuffer;//Buffer passed for each write op
    OVERLAPPEDPLUS *pWriteOverlap;
#endif
    DataBuffer oBuffer;
    BOOL bWriteInProgress;

    SOCKET socket;
    UINT rPeerPort;
    std::string rPeerIP;

    bool WriteBytes();
    void CloseSocket();

private:
    ConnectionContext* pConnectionContext;
public:
    void SetConnectionContext(ConnectionContext* pConnectionContext);
    ConnectionContext* GetConnectionContext();
    void attachToClient(LogicalConnection* pLogicalConnection);
    void attachToClient(std::string clientKey);
    SOCKET getSocket();
    //
public:
    BroadcastContext broadcastCxt;

};
typedef std::vector<PhysicalConnection*> vectConnectionsT;

}

#endif // PhysicalConnection__INCLUDED
