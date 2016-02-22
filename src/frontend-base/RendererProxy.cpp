#include <thread>

#include "commands/ProcessingCommands.h"

#include "RendererProxy.h"

using namespace trinity::frontend;
using namespace trinity::common;

RendererProxy::RendererProxy(std::shared_ptr<VisStream> s, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                             const unsigned int& sid)
    : IRenderer(s)
    , IProxy(controlEndpoint)
    , m_visReceiver(std::move(visEndpoint), s)
    , m_remoteSessionId(sid) {}

RendererProxy::~RendererProxy() {
    m_visReceiver.endStreaming();
}


int RendererProxy::getRemoteSessionId() const {
    return m_remoteSessionId;
}

void RendererProxy::setIsoValue(const float value) {
    commands::SetIsoValueCmd cmd(m_remoteSessionId, m_ridGen.nextID(), value);

    // send cmd, receive reply
    m_inputChannel.sendCommand(cmd);
}