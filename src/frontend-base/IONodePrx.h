#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "common/Commands.h"
#include "common/CommandInputChannel.h"


namespace trinity {
namespace frontend {
    
class IONodePrx {
    
public:
    /// see mocca::net::Endpoint for details, supports remote and local calls
    IONodePrx(const mocca::net::Endpoint&);
    ~IONodePrx();
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    
    
    
private:
    common::CommandInputChannel m_inputChannel;
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;


};
}
}