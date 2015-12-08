#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IProtocolConnection.h"
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
    
    /// true on success. Does not throw any errors
    bool connect();
    void disconnect();
    
    std::unique_ptr<RendererPrx> spawnRenderer(const std::string&);
    
private:
    const mocca::net::Endpoint m_processingNode;
    std::unique_ptr<mocca::net::IProtocolConnection> m_mainChannel;
    trinity::IDGenerator m_mainChannelIDGen;
    std::atomic<bool> m_exitFlag;
};
}