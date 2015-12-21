#include <thread>
#include "RendererPrx.h"
#include "common/MuiError.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"
#include "common/NetConfig.h"

using namespace trinity;

RendererPrx::RendererPrx(mocca::net::Endpoint ep, const unsigned int& sid) :
m_endpoint(ep), m_sid(sid) {
}


bool RendererPrx::connect() {
    LINFO("(f) connecing to remote renderer at " << m_endpoint);
    try {
        m_mainChannel = mocca::net::ConnectionFactorySelector::connect(m_endpoint);
    } catch (const mocca::net::NetworkError&) {
        LERROR("(f) no connection to render session  at \"" << m_endpoint << "\": ");
        return false;
    }
    LINFO("(f) successfully connected to render session at \"" << m_endpoint << "\": ");
    return true;
}

int RendererPrx::getFrameBuffer() {
    
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
    return 2;
}