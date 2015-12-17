#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ByteArray.h"
#include <iostream>
#include <memory>

using namespace mocca::net;

int main(int argc, char** argv)
{
    
    ConnectionFactorySelector::addDefaultFactories();

    auto acceptor = ConnectionFactorySelector::bind(Endpoint(ConnectionFactorySelector::tcpPrefixed(), "5678"));
    // acceptor.getConnection();
    while (true) {
        auto conn = acceptor->accept();
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