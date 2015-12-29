#pragma once
#include <memory>
#include "mocca/net/Endpoint.h"
#include "common/IRenderer.h"
#include "mocca/net/IMessageConnection.h"
#include "common/Commands.h"
#include "VisStreamReceiver.h"

namespace trinity {
namespace frontend {
    
class RendererPrx : public common::IRenderer {
    
public:
    
    /// local proxy to a remote render session
    RendererPrx(std::shared_ptr<common::VisStream> stream,
                mocca::net::Endpoint controlEndpoint,
                mocca::net::Endpoint visEndpoint,
                const unsigned int& sessionId);
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    
    
private:
    VisStreamReceiver m_visReceiver;
    mocca::net::Endpoint m_controlEndpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
    const unsigned int m_sid;
    

    
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;
};
}
}