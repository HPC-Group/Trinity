#pragma once

#include "mocca/net/ConnectionFactorySelector.h"

#include <memory>
#include <chrono>

namespace trinity {

static const std::chrono::milliseconds TIMEOUT_DEFAULT(100);
static const int RENDERSESSION_BASE_PORT = 5999;
static const int VISRECEIVER_BASE_PORT = 6999;
static const mocca::net::Endpoint& ioLoopbackEndpoint() {
    static mocca::net::Endpoint endpoint(mocca::net::ConnectionFactorySelector::loopback(), "localhost", "ioloop");
    return endpoint;
}
}