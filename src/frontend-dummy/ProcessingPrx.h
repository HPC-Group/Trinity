#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "RendererPrx.h"
#include "common/Commands.h"

#include <memory>
#include <atomic>

namespace trinity {
    
    class ProcessingPrx {
    
public:

    
    /// see mocca::net::Endpoint for details, supports remote and local calls
    ProcessingPrx(mocca::net::Endpoint);
    
    ~ProcessingPrx();
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    
    std::unique_ptr<RendererPrx> spawnRenderer(const std::string&);
    
private:
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
    trinity::IDGenerator m_mainChannelIDGen;
};
}