#pragma once
#include <memory>
#include "mocca/net/Endpoint.h"
#include "common/IRenderer.h"
#include "mocca/net/IMessageConnection.h"
#include "common/Commands.h"
#include "common/CommandInputChannel.h"
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
    ~RendererPrx();
    
    virtual void setIsoValue(const float);
    int getRemoteSessionId() const;
    
    
private:
    VisStreamReceiver m_visReceiver;
    mocca::net::Endpoint m_controlEndpoint;
    common::CommandInputChannel m_inputChannel;
    const unsigned int m_sid;
    static int m_basePort;
    

    
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;
};
}
}