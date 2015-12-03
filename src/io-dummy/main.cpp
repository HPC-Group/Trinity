#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/MoccaNetworkService.h"
#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ByteArray.h"
#include <iostream>
#include <memory>

using namespace mocca::net;

int main(int argc, char** argv)
{
    
    NetworkServiceLocator::provideService(std::make_shared<MoccaNetworkService>(
        std::unique_ptr<IPhysicalNetworkService>(new TCPNetworkService())));

    auto acceptor = NetworkServiceLocator::bind(Endpoint(MoccaNetworkService::protocolStatic(), TCPNetworkService::transportStatic(), "5678"));
    // acceptor.getConnection();
    while (true) {
        auto conn = acceptor->getConnection();
        if(conn) {
            std::cout << "connection opened" << std::endl;
            while(true) {
                auto byteArray = conn->receive();
                if (!byteArray.isEmpty()) {
                    std::cout << byteArray.read(byteArray.size()) << std::endl;
                } else
                    std::cout << "empty" << std::endl;
            }
        }
    }
    
}