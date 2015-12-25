#pragma once
#include <memory>
#include "mocca/net/Endpoint.h"
#include "common/IRenderer.h"
#include "mocca/net/IMessageConnection.h"
#include "common/Commands.h"

namespace trinity {
namespace frontend {
    
class RendererPrx : public common::IRenderer {
    
public:
    
    /// local proxy to a remote render session
    RendererPrx(std::shared_ptr<common::VisStream> stream,
                mocca::net::Endpoint remoteRenderer,
                const unsigned int& sessionId);
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    
    /// asks the remote renderer to update the video stream, blocking call
    void updateStream();
    
private:
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
    const unsigned int m_sid;
    
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;
};
}
}