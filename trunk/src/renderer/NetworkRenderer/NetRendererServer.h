#ifndef TNG_NETRENDERERSERVER_h
#define TNG_NETRENDERERSERVER_h

#include <renderer/RenderManager.h> //RenderManager!

#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

using namespace mocca;
using namespace mocca::net;
using namespace Tuvok::Renderer;

typedef std::shared_ptr<IRenderer> RenderPtr;

namespace Tuvok{
    namespace Renderer{

    class NetRendererServer{
        public:
        NetRendererServer();
        ~NetRendererServer();

        void openServer(int port);
        void acceptConnection();
        void waitForMsg();

        protected:
        void handleMsg(std::string msg);

        private:
        RenderPtr                           renderer;
        uint64_t                            _lastFrameID;
        std::vector<uint8_t>                     _compressedData;
        std::unique_ptr<IConnectionListener> connectionListener;
        std::unique_ptr<AbstractConnection> serverConnection;
        int connectionCounter;

    };

    };
};

#endif
