#include <thread>
#include "RendererPrx.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"
#include "common/NetConfig.h"

using namespace trinity::frontend;
using namespace trinity::common;

int RendererPrx::m_basePort = 6990; // config base port for renderer here

RendererPrx::RendererPrx(std::shared_ptr<VisStream> s,
                         mocca::net::Endpoint controlEndpoint,
                         mocca::net::Endpoint visEndpoint,
                         const unsigned int& sid) :
m_visReceiver(std::move(visEndpoint), s),
IRenderer(s),
m_controlEndpoint(controlEndpoint),
m_sid(sid) {
}


bool RendererPrx::connect() {
    LINFO("(f) connecing to remote renderer at " << m_controlEndpoint);
    try {
        m_mainChannel = mocca::net::ConnectionFactorySelector::connect(m_controlEndpoint);
    } catch (const mocca::net::NetworkError&) {
        LERROR("(f) no connection to render session at \"" << m_controlEndpoint);
        return false;
    }
    LINFO("(f) connected to render session at \"" << m_controlEndpoint);
    return true;
}

/*

VideoPipeline& RendererPrx::getVideoPipeline() {
    
    std::string cmd = m_vcl.assembleGetFrameBuffer(m_sid, m_ridGen.nextID());
    m_mainChannel->send(std::move(mocca::ByteArray()<< cmd));

    
    auto byteArray = m_mainChannel->receive(trinity::config::TIMEOUT_REPLY);
    
    if(byteArray.isEmpty()) {
        throw mocca::Error("cannot get framebuffer, no reply arrived",
                           __FILE__, __LINE__);
    }
    
    std::string rep = byteArray.read(byteArray.size());
    LINFO("(f) cmd : " << cmd << "; reply: " << rep);
    // std::vector<std::string> args = mocca::splitString<std::string>(rep, '_');
    return m_remoteBuffer;
}
*/