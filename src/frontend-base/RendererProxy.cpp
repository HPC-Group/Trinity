#include "frontend-base/RendererProxy.h"

#include "commands/ProcessingCommands.h"

#include <thread>

using namespace trinity::frontend;
using namespace trinity::common;
using namespace trinity::commands;


RendererProxy::RendererProxy(std::shared_ptr<VisStream> s, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                             unsigned int sid)
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
    SetIsoValueCmd::RequestParams requestParams(value);
    SetIsoValueRequest request(requestParams, m_ridGen.nextID(), m_remoteSessionId);

    m_inputChannel.sendRequest(request);
}