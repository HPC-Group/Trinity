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
    , m_remoteSessionId(sid) {
        m_visReceiver.startStreaming();
    }

RendererProxy::~RendererProxy() {
    m_visReceiver.endStreaming();
}

int RendererProxy::getRemoteSessionId() const {
    return m_remoteSessionId;
}

#include <iostream>

void RendererProxy::setIsoValue(const float value) {
    SetIsoValueCmd::RequestParams requestParams(value);
    SetIsoValueRequest request(requestParams, m_ridGen.nextID(), m_remoteSessionId);

    std::cout << m_inputChannel.getEndpoint().toString() << std::endl;
    m_inputChannel.sendRequest(request);
}

void RendererProxy::initContext()  {
    InitContextCmd::RequestParams requestParams(1);
    InitContextRequest request(requestParams, m_ridGen.nextID(), m_remoteSessionId);

    std::cout << m_inputChannel.getEndpoint().toString() << std::endl;
    m_inputChannel.sendRequest(request);
}
