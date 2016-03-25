#include "frontend-base/RendererProxy.h"

#include "commands/ProcessingCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include <thread>

using namespace trinity;

RendererProxy::RendererProxy(std::shared_ptr<VisStream> s, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                             unsigned int sid)
    : IRenderer(s)
    , m_inputChannel(controlEndpoint)
    , m_visReceiver(std::move(visEndpoint), s)
    , m_remoteSessionId(sid) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to processing node", __FILE__, __LINE__);
    }
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
    SetIsoValueRequest request(requestParams, IDGenerator::nextID(), m_remoteSessionId);

    m_inputChannel.sendRequest(request);
}

void RendererProxy::zoomCamera(float f) {
    // todo
    ZoomCameraCmd::RequestParams requestParams(f);
    ZoomCameraRequest request(requestParams, IDGenerator::nextID(), m_remoteSessionId);
    
    m_inputChannel.sendRequest(request);
}


void RendererProxy::initContext() {
    InitContextCmd::RequestParams requestParams(1);
    InitContextRequest request(requestParams, IDGenerator::nextID(), m_remoteSessionId);

    m_inputChannel.sendRequest(request);
}

#define PYTHON_MAGIC


#undef PYTHON_MAGIC