#pragma once
#include <memory>
#include "mocca/net/Endpoint.h"
#include "common/IRenderer.h"
#include "mocca/net/IMessageConnection.h"
#include "common/Commands.h"

namespace trinity {
    
class RendererPrx : public IRenderer {
    
public:
    
    /// local proxy to a remote render session
    RendererPrx(mocca::net::Endpoint, const unsigned int& sid);
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    
    int getFrameBuffer();
    
    
private:
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
    trinity::IDGenerator m_ridGen;
    const unsigned int m_sid;
    Vcl m_vcl;
};
}