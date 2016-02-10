#include <thread>
#include "RendererPrx.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"
#include "common/NetConfig.h"
#include "common/SimpleRenderingCmds.h"

using namespace trinity::frontend;
using namespace trinity::common;

int RendererPrx::m_basePort = 6990; // config base port for renderer here

RendererPrx::RendererPrx(std::shared_ptr<VisStream> s,
                         mocca::net::Endpoint controlEndpoint,
                         mocca::net::Endpoint visEndpoint,
                         const unsigned int& sid) :
IRenderer(s),
m_visReceiver(std::move(visEndpoint), s),
m_controlEndpoint(controlEndpoint),
m_inputChannel(m_controlEndpoint),
m_sid(sid) {
}

RendererPrx::~RendererPrx() {
    m_visReceiver.endStreaming();
}


bool RendererPrx::connect() {
    LINFO("(f) connecing to remote renderer at " << m_controlEndpoint);
    return m_inputChannel.connect();
}

int RendererPrx::getRemoteSessionId() const {
    return m_sid;
}

void RendererPrx::setIsoValue(const float value) {
    SetIsoValueCmd cmd(m_sid, m_ridGen.nextID(), value);
    
    // send cmd, receive reply
    m_inputChannel.sendCommand(cmd);
}