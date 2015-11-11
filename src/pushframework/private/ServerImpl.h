/********************************************************************
	File :			ServerImpl.h
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
#ifndef ServerImpl__INCLUDED
#define ServerImpl__INCLUDED

#pragma once




namespace PushFramework
{

class Protocol;
class OutgoingPacket;
class ClientFactory;
class Service;
class BroadcastManager;
class MonitorProtocol;
class Demultiplexor;
class Dispatcher;
class ServerStats;
class ChannelFactory;
class IOQueue;
class BroadcastManagerImpl;
class ClientFactoryImpl;
class Acceptor;
class MonitorAcceptor;
class Server;
class GarbageCollector;
struct Version;
struct ListenerOptions;

class ServerImpl
{
public:
    ServerImpl(Server* pFacade);
    ~ServerImpl(void);

    void setServeInfos(const char* serverInfos);
    string getServerInfos();
    void setProtocol(Protocol* pProtocol);
    void setClientFactory(ClientFactory* pClientFactory);
    bool createListener( int port, ListenerOptions* pOptions /*= NULL*/ );

    void enableRemoteMonitor(short sPort, const char* password);
    void enableProfiling(unsigned int samplingRate);
    void disableProfiling();
    bool getProfilingStatus();

    void setMaxConnections(int nMaxConnections);


    void setLoginExpiryDuration( unsigned int uLoginExpireDuration );

    void setWorkerCount(int workerCount);
    void registerService(unsigned int serviceId, Service* pService, const char* serviceName);
    void start(bool startInSeparateThread);
    void stop();
    void pause();
    void resume();
    ClientFactory* getClientFactory();

    unsigned int getSamplingRate();
    string getMonitorPassword();

    int getWorkerCount();


    Server* getFacade();
    void getVersion(Version& ver);



private:
    Server* pFacade;
    string serverInfos;

    // User defined
    Protocol* pProtocol;
    ClientFactory* pClientFactory;
    short listeningPort;
    int workerCount;

    // Remote monitoring
    bool isMonitorEnabled;
    bool isProfilingEnabled;
    short monitorPort;
    unsigned int samplingRate;
    string password;

#ifdef Plateform_Windows
    HANDLE			m_hEventTimer;	// Used for sleeping during interval period.
    HANDLE			m_hEventAbort;// set by stop function.
    //In case of an asynchronous start :
    HANDLE hThread;
    static DWORD WINAPI threadProc(LPVOID WorkContext);
#else
#ifdef Plateform_Linux
    pthread_cond_t abortCond;// set by stop function.
    //In case of an asynchronous start :
    pthread_t hThread;
    static void* threadProc(void* WorkContext);
#endif
#endif


    unsigned int uGCPeriod;
    void eternal_loop();


    //listeners with their options :
    typedef std::map<int, Acceptor*> listenersMapT;
    listenersMapT listenersMap;
    //
    bool startListeners();
    void stopListeners();
    void assignDefaultProtocol();
public:
    void EncodeOnAllProtocols(OutgoingPacket& outgoingPacket);


private:
    int nSecsGCCount;
    int nSecsPerfObservCount;

    void DoPeriodicJobs();

};


extern ServerImpl *pServerImpl;

}
#endif // ServerImpl__INCLUDED

