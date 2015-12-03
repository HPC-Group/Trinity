#include <sstream>

#include "ProcessingPrx.h"
#include "common/MuiError.h"
#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/Error.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"

using namespace trinity;


ProcessingPrx::~ProcessingPrx() {
    m_exitFlag = true;
    LINFO("dtor");
}

ProcessingPrx::ProcessingPrx(const mocca::net::Endpoint& endpoint) :
m_processingNode(endpoint), m_exitFlag(false) {}


bool ProcessingPrx::connect() {
    
    try {
        m_mainChannel = mocca::net::NetworkServiceLocator::connect(m_processingNode);
    } catch (const mocca::net::ConnectFailedError& err) {
        LWARNING("no connection to processing  at \"" << m_processingNode << "\": ");
        return false;
    }
    std::stringstream cmd;
    cmd << trinity::vcl::INIT_CONNECTION
    << "_" << std::to_string(0) << "_" <<  m_mainChannelIDGen.nextID();
    auto cmdStr = cmd.str();
    m_mainChannel->send(mocca::ByteArray::createFromRaw(cmdStr.c_str(), cmdStr.size()));
    LINFO("successfully connected to processing at \"" << m_processingNode << "\": ");
    return true;
}

void ProcessingPrx::disconnect() {

}

std::unique_ptr<RendererPrx> ProcessingPrx::spawnRenderer() {
    
    std::stringstream cmd;
    cmd << trinity::vcl::INIT_RENDERER
    << "_" << std::to_string(0) << "_" <<  m_mainChannelIDGen.nextID();
    auto cmdStr = cmd.str();
    m_mainChannel->send(mocca::ByteArray::createFromRaw(cmdStr.c_str(), cmdStr.size()));
    
    while(!m_exitFlag) {
        auto byteArray = m_mainChannel->receive();
        LINFO("cmd : " << cmd.str() << "; reply: " << byteArray.read(byteArray.size()));
    }
    
    
}