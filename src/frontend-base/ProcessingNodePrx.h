#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "RendererPrx.h"
#include "common/Commands.h"

#include <memory>
#include <atomic>

namespace trinity {
namespace frontend {
    
    class ProcessingNodePrx {
    
public:

    
    /// see mocca::net::Endpoint for details, supports remote and local calls
    ProcessingNodePrx(mocca::net::Endpoint);
    
    ~ProcessingNodePrx();
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    std::unique_ptr<RendererPrx> initRenderer(const common::VclType&);
    
private:
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
        
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;
};
}
}