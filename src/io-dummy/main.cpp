#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/IConnectionListener.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/AbstractConnection.h"
#include "mocca/base/ByteArray.h"
#include <iostream>
#include <memory>

using namespace mocca::net;

int main(int argc, char** argv)
{
    
    NetworkServiceLocator locator;
    locator.provideService(std::make_shared<TCPNetworkService>());
    auto listener = locator.bind(Endpoint(TCPNetworkService::transportStatic(), "5678"));
    // listener.getConnection();
    while (true) {
        auto conn = listener->getConnection();
        if(conn) {
            std::cout << "connection opened" << std::endl;
            while(true) {
                auto byteArray = conn->receive();
                if (!byteArray.isEmpty()) {
                    std::cout << byteArray.get<std::string>() << std::endl;
                } else
                    std::cout << "empty" << std::endl;
            }
        }
    }
    
}