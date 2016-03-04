#pragma once

#include "common/IOSessionProxy.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

namespace trinity {
namespace common {

class IONodeProxy {

public:
    IONodeProxy(const mocca::net::Endpoint& ep);

    std::unique_ptr<IOSessionProxy> initIO(int fileId);

    // list data goes here

private:
    commands::CommandInputChannel m_inputChannel;
};
}
}