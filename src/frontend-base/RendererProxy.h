#pragma once
#include <memory>

#include "common/IProxy.h"
#include "common/IRenderer.h"

#include "VisStreamReceiver.h"

namespace trinity {
namespace frontend {


class RendererProxy : public common::IRenderer, public common::IProxy {

public:
    /// local proxy to a remote render session
    RendererProxy(std::shared_ptr<common::VisStream> stream, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                  const unsigned int& sessionId);


    virtual ~RendererProxy();

    virtual void setIsoValue(const float);
    int getRemoteSessionId() const;


private:
    VisStreamReceiver m_visReceiver;
    int m_remoteSessionId;
};
}
}