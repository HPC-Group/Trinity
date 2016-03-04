#pragma once
#include <memory>

#include "common/IRenderer.h"
#include "commands/CommandInputChannel.h"
#include "frontend-base/VisStreamReceiver.h"

namespace trinity {
namespace frontend {


class RendererProxy : public common::IRenderer {

public:
    /// local proxy to a remote render session
    RendererProxy(std::shared_ptr<common::VisStream> stream, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                  unsigned int sessionId);


    virtual ~RendererProxy();

    virtual void setIsoValue(const float);
    virtual void initContext();
    int getRemoteSessionId() const;


private:
    commands::CommandInputChannel m_inputChannel;
    VisStreamReceiver m_visReceiver;
    int m_remoteSessionId;
};
}
}
