#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "IOSessionProxy.h"
#include "common/IProxy.h"

namespace trinity {
namespace common {

class IONodeProxy : public common::IProxy {

public:
    IONodeProxy(const mocca::net::Endpoint& ep)
        : common::IProxy(ep) {}

    std::unique_ptr<IOSessionProxy> initIO(int fileId);

    // list data goes here
};
}
}