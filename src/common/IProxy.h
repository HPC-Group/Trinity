#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "commands/CommandInputChannel.h"
#include "commands/Vcl.h"


namespace trinity {
namespace common {

// dmc: does IProxy really define a genuine interface? is it really used in a rumtime polymorphic way?
class IProxy {

public:
    /// see mocca::net::Endpoint for details, supports remote and local calls
    IProxy(const mocca::net::Endpoint& endpoint)
        : m_inputChannel(endpoint) {}
    virtual ~IProxy() {}

    /// true on success. Does not throw any errors. Disconnect happens in dtor
    virtual bool connect() { return m_inputChannel.connect(); }

protected:
    commands::CommandInputChannel m_inputChannel;
    commands::IDGenerator m_ridGen;
};
}
}