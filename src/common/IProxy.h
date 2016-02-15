#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "commands/Vcl.h"
#include "commands/CommandInputChannel.h"


namespace trinity {
namespace common {
    
class IProxy {
    
public:
    /// see mocca::net::Endpoint for details, supports remote and local calls
    IProxy(const mocca::net::Endpoint& endpoint) : m_inputChannel(endpoint) {}
    virtual ~IProxy() {}
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    virtual bool connect() { return m_inputChannel.connect(); }
    
protected:
    commands::CommandInputChannel m_inputChannel;
    commands::IDGenerator m_ridGen;
    commands::Vcl m_vcl;
    
};
}
}