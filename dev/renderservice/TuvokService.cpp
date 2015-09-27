#include "TuvokService.h"
#include <core/splittools.h>

#include <renderer/NetworkRenderer/NetCommands.h>

#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

#include <core/splittools.h>

using namespace ghoul::logging;
using namespace Tuvok::Renderer;


TuvokService::TuvokService(std::string port):
_connectionCounter(0),
_disconnectedConnections(),
_portCounter(41420)
{
    TCPNetworkService netService;
    _listener = netService.bind(port);

    _connectionThread = std::unique_ptr<std::thread>(new std::thread(&TuvokService::acceptConnection,this));
}

TuvokService::~TuvokService()
{
    //dtor
}

void TuvokService::run(){
    while(true){
        acceptMsg();
    }
}


void TuvokService::acceptConnection(){
    while(true){
       std::unique_ptr<AbstractConnection> con = _listener->getConnection();
        if(con != nullptr){
            _connections.insert(std::pair<  uint32_t,
                                        std::unique_ptr<AbstractConnection>>
                                        (++_connectionCounter,std::move(con)));

            LINFOC("Renderservice", "new connection to service");
        }
    }
}

void TuvokService::acceptMsg(){
    _disconnectedConnections.clear();
    for (auto connection = _connections.begin(); connection!=_connections.end(); ++connection){
        try{
            BytePacket data = connection->second->receive();
            if(data.byteArray().size() > 0){
                std::string s(data.byteArray().data(),0,data.byteArray().size());

                LDEBUGC("Renderservice", s);
                handleMsg(s,connection->first);
            }
        }catch(const ConnectionClosedError& err){
            _disconnectedConnections.push_back(connection->first);
        }catch(const NetworkError& err){
            _disconnectedConnections.push_back(connection->first);
        }
    }
    //not sure if the iterator will fail if we delet on err
    for(uint32_t i : _disconnectedConnections){
        _connections.erase(i);
    }
}

void TuvokService::handleMsg(std::string msg, uint32_t id){
    //split the msg into a vector!
    std::vector<std::string> args = splitString(msg,':');

    if(args.size() <= 0) return;

    if(args[0] == Commands::OPENTICKET && args.size() == 5){
        LINFOC("Renderservice", "connection requests new renderer");
        //OPENTICKET:FILENAME:TRANSFERFUNKTION:AUFLÖSUNGX:AUFLÖSUNGY ---return---> port

#ifdef _WIN32
		std::string cmd = "START cmd /c Renderprocess " + std::to_string(_portCounter) + " " + args[1] + " " + args[2] + " " + args[3] + " " + args[4];
#else
        std::string cmd = "nohup setsid ./Renderprocess "+ std::to_string(_portCounter) +" "+ args[1]+" "+ args[2]+" "+ args[3]+" "+ args[4]+" &";
#endif
        LINFOC("Renderservice", "starting : " << cmd);

        system(cmd.c_str());

        ByteArray PortArray;
        PortArray.append(&_portCounter,sizeof(_portCounter));

        LINFOC("Renderservice", "renderer runs with port "<< _portCounter);
        _connections.find(id)->second->send(PortArray);

		_portCounter = _portCounter + 2;

        if(_portCounter > 51414){
            _portCounter = 41414;
        }

        _connections.erase(id);
    }
}

int TuvokService::startRenderProcess(){
	return 0;
}
