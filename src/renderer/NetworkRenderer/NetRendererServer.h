#ifndef TNG_NETRENDERERSERVER_h
#define TNG_NETRENDERERSERVER_h

#include <renderer/RenderManager.h> //RenderManager!

#include "NetCommands.h"

#include <base/Error.h>
#include <base/ByteArray.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

#include <map>

#include <core/Time/Timer.h>

#include <thread>

using namespace mocca;
using namespace mocca::net;
using namespace Tuvok::Renderer;

typedef std::shared_ptr<IRenderer> RenderPtr;

namespace Tuvok{
    namespace Renderer{


    class Ticket{
    public:
        Ticket(uint32_t ticketID, double TTLdelta = 60.0f):
            _ticketID(ticketID),
            _renderer(nullptr),
            _ticketTimer(),
            _TTLdelta(TTLdelta)
            {
            _ticketTimer.start();
            _lastPing = _ticketTimer.now();
            };
        ~Ticket(){
            _renderer->stopRenderThread();
            _renderer = nullptr;
        };

        RenderPtr getRenderer() const { return _renderer;}
        void setRenderer(RenderPtr p) { _renderer = p;}
        int32_t getTicketId() const { return _ticketID; }

        void cleanup(){};

        void updatePing(){_lastPing = _ticketTimer.now();}
        const bool endOfLife() {
            return (_ticketTimer.now()-_lastPing) > _TTLdelta ? true : false;
            }

    private:
        uint32_t                                            _ticketID;
        RenderPtr                                           _renderer;
        Core::Time::Timer                                   _ticketTimer;
        double                                              _lastPing;
        double                                              _TTLdelta;

        //specifics
        std::vector<uint8_t>                                _compressedData;
    };


    class NetRendererServer{
        public:
        NetRendererServer();
        ~NetRendererServer();

        void openServer(int port);
        void waitForMsg();

        void acceptConnection();
        void acceptionThreadMth();

        protected:
        void handleMsg(std::string msg, uint32_t connectionID);
        void cleanOldConnections();
        void cleanOldTickets();

        private:
        //RenderPtr                                                   renderer;
        std::vector<uint32_t>                                       _disconnectedConnections;
        std::vector<int32_t>                                        _endoflifeTickets;

        std::vector<uint8_t>                                        _compressedData;
        std::unique_ptr<IConnectionListener>                        connectionListener;
        std::shared_ptr<std::thread>                                _acceptionThread;



        private:
        RenderPtr getRenderer(uint32_t ticketID) const;
        void updateTTL(uint32_t ticketID);
        int32_t                                                     _ticketCounter;
        std::map<int32_t, std::unique_ptr<Ticket>>                  _tickets;

        uint32_t                                                    _connectionCounterID;
        std::map<uint32_t,std::unique_ptr<AbstractConnection>>      _connections;


        //rendererFunctions
        private:
        void OpenTicket(uint32_t& connectionID, std::vector<std::string>& args);
        void InitRenderer(uint32_t& connectionID, int32_t& ticketID, std::vector<std::string>& args);

        void ReadFramebuffer(RenderPtr renderer,uint32_t& connectionID, int32_t& ticketID, std::vector<std::string>& args);
        void RotateCamera(RenderPtr renderer,uint32_t& connectionID, int32_t& ticketID, std::vector<std::string>& args);
        void MoveCamera(RenderPtr renderer,uint32_t& connectionID, int32_t& ticketID, std::vector<std::string>& args);


    };

    };
};

#endif
