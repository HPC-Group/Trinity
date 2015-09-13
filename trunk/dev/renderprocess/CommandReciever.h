#ifndef COMMANDRECIEVER
#define COMMANDRECIEVER


#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>
#include <net/Error.h>

#include <renderer/IRenderer.h>

#include <thread>


using namespace mocca;
using namespace mocca::net;
using namespace Tuvok::Renderer;


class CommandReciever{
public:
    CommandReciever(uint32_t port = 42420, std::shared_ptr<IRenderer> renderer = nullptr);
    virtual ~CommandReciever();

    void setRenderer(std::shared_ptr<IRenderer> renderer);

    void endCommandReciever();

    void run();

    void startThread();

protected:
    void handleMsg(std::string& msg);


private:
    std::unique_ptr<IConnectionListener>    _listener;
    std::unique_ptr<AbstractConnection>     _connection;

    std::shared_ptr<IRenderer>              _renderer;

    uint32_t                                _port;

    bool                                    _keepAlive;

    std::unique_ptr<std::thread>            _thread;
};


#endif // COMMANDRECIEVER
