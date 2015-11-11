#ifndef TUVOKSERVICE_H
#define TUVOKSERVICE_H

#include <memory>
#include <vector>

#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>
#include <net/WSNetworkService.h>
#include <net/Error.h>

#include <thread>

#include <stdio.h>
#include <stdlib.h>

#include <mocca\include\mocca\net\WebsocketProtocol.h>


using namespace mocca;
using namespace mocca::net;


class TuvokService
{
    public:
        TuvokService(std::string port = "1234");
        virtual ~TuvokService();

        void run();

    protected:
        void handleMsg(std::string msg, uint32_t id);

    private:
        void acceptConnection();
        void acceptMsg();

        int startRenderProcess();

    private:
         std::unique_ptr<IConnectionListener>                       _listener;

         uint32_t                                                   _connectionCounter;
         std::map<uint32_t,std::unique_ptr<AbstractConnection>>     _connections;

         int32_t                                                    _portCounter;

         std::unique_ptr<std::thread>                               _connectionThread;

         std::vector<int32_t>                                       _disconnectedConnections;

		 WebsocketProtocol											_protocol;
};

#endif // TUVOKSERVICE_H
