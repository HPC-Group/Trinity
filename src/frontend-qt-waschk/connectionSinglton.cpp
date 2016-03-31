#include "connectionSinglton.h"

using namespace std;
using namespace mocca::net;

static int reconnectInSec = 5;

ConnectionSingleton::ConnectionSingleton():
_ioNode(nullptr),
_processingNode(nullptr){

}


ConnectionSingleton::~ConnectionSingleton(){
_ioNode = nullptr;
_processingNode = nullptr;
}

trinity::IONodeProxy& ConnectionSingleton::ioNode(){
    return *_ioNode;
}

trinity::ProcessingNodeProxy& ConnectionSingleton::processingNode(){
    return *_processingNode;
}


void ConnectionSingleton::connectIO(const std::string& hostname, const std::string& port){
    std::cout << hostname << " "<< port << std::endl;
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(),
                        hostname,
                        port);
    _ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
}

void ConnectionSingleton::connectProcessing(const std::string& hostname, const std::string& port){
    std::cout << hostname << " "<< port << std::endl;
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(),
                      hostname,
                      port);

    _processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
}


