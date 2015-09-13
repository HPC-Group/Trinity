#include "CommandReciever.h"

#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

#include <renderer/NetworkRenderer/NetCommands.h>

#include <core/splittools.h>
#include <core/Math/Vectors.h>

using namespace ghoul::logging;
using namespace Core::Math;

CommandReciever::CommandReciever(uint32_t port, std::shared_ptr<IRenderer> renderer):
_listener(nullptr),
_connection(nullptr),
_renderer(renderer),
_keepAlive(true),
_port(port),
_thread(nullptr)
{
    TCPNetworkService netService;
    _listener  = netService.bind(std::to_string(port)); // listen for connections
}


CommandReciever::~CommandReciever(){

}

void CommandReciever::setRenderer(std::shared_ptr<IRenderer> renderer){
    _renderer = renderer;
}


void CommandReciever::run(){
    //wait for client to connect
    while(_connection == nullptr && _keepAlive){
        _connection = _listener->getConnection();
        std::cout <<" waiting" << std::endl;
    }


    while(_keepAlive){
        try{
            BytePacket data = _connection->receive();
            if(data.byteArray().size() > 0){
                std::string s(data.byteArray().data(),0,data.byteArray().size());
                LDEBUGC("Renderprocess", s);

                handleMsg(s);
            }

        }catch(const ConnectionClosedError& err){
            LERRORC("Renderprocess", "lost connection");
            _keepAlive = false;
            _renderer->stopRenderer();
        }catch(const NetworkError& err){
            LERRORC("Renderprocess", "network error");
            _keepAlive = false;
            _renderer->stopRenderer();
        }
    }
}

void CommandReciever::handleMsg(std::string& msg){
    int32_t ticketID = 0;

    //split the msg into a vector!
    std::vector<std::string> args = splitString(msg,':');

    if(args.size() <= 0) return;

    if(_renderer != nullptr){
        if(args.size() >= 3)
            ticketID = std::atoi(args[1].c_str());

        //requested framebuffer?
        if(args[0] == Commands::FRAME && args.size() == 4){ //request frame needs 1 parameter + 2 overhead

            uint64_t clientFrame = std::atoi(args[3].c_str());

            FrameData frame  = _renderer->ReadFrameBuffer();

            //send the frameID;
            ByteArray framePacket;
            framePacket.append(&(frame._frameID),sizeof(frame._frameID));
            _connection->send(framePacket);

            //does the client have an older frame?
            if(clientFrame < frame._frameID){
                ByteArray p;
                p.append(&(frame._data[0]),frame._data.size());
                _connection->send(p);
            }

        //rotate renderer
        }else if(args[0] == Commands::ROT && args.size() == 6){

            _renderer->RotateCamera(Vec3f(   std::atof(args[3].c_str()) ,
                                             std::atof(args[4].c_str()) ,
                                             std::atof(args[5].c_str()) ));
        }
    }
}


void CommandReciever::endCommandReciever(){
    _keepAlive= false;
}

void CommandReciever::startThread(){
    _thread = std::unique_ptr<std::thread>(new std::thread(&CommandReciever::run,this));
}

void CommandReciever::joinThread(){
	_thread->join();
}