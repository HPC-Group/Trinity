#include <sstream>

#include "ProcessingPrx.h"
#include "common/MuiError.h"
#include <thread>

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/Error.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"
#include "common/NetConfig.h"


using namespace trinity;

static std::chrono::milliseconds receiveTimeout(50);

ProcessingPrx::~ProcessingPrx() {}

ProcessingPrx::ProcessingPrx(mocca::net::Endpoint endpoint) :
m_endpoint(endpoint) {}


bool ProcessingPrx::connect() {
    
    try {
        m_mainChannel = mocca::net::NetworkServiceLocator::connect(m_endpoint);
    } catch (const mocca::net::ConnectFailedError& err) {
        LWARNING("no connection to processing  at \"" << m_endpoint << "\": ");
        return false;
    }
    std::stringstream cmd;
    cmd << trinity::vcl::INIT_CONNECTION
    << "_" << std::to_string(0) << "_" <<  m_mainChannelIDGen.nextID();
    auto cmdStr = cmd.str();
    m_mainChannel->send(mocca::ByteArray::createFromRaw(cmdStr.c_str(), cmdStr.size()));
    LINFO("successfully connected to processing at \"" << m_endpoint << "\": ");
    return true;
}


std::unique_ptr<RendererPrx> ProcessingPrx::spawnRenderer(const std::string& type) {
    
    std::stringstream cmd;
    cmd << trinity::vcl::INIT_RENDERER
    << "_" << 0 << "_" <<  m_mainChannelIDGen.nextID() << "_" << type;
    auto cmdStr = cmd.str();
    m_mainChannel->send(mocca::ByteArray::createFromRaw(cmdStr.c_str(), cmdStr.size()));
    
    auto byteArray = m_mainChannel->receive(trinity::config::TIMEOUT_REPLY);
    
    if(byteArray.isEmpty()) {
        throw mocca::Error("cannot spawn renderer, no reply arrived",
                           __FILE__, __LINE__);
    }
                           
    std::string rep = byteArray.read(byteArray.size());
    LINFO("cmd : " << cmd.str() << "; reply: " << rep);
    std::vector<std::string> args = mocca::splitString<std::string>(rep, '_');
    
    
    if (args.size() < vcl::MIN_CMD_LENGTH) {  // cmd not well-formatted
        throw mocca::Error("cannot spawn renderer, result has wrong format",
                           __FILE__, __LINE__);
    }
    
    if(args[0] == vcl::TRI_ERROR) {
        throw mocca::Error("cannot spawn renderer, error code: " + args[3],
                           __FILE__, __LINE__);
    }
    
    // result is a valid renderer
    // todo
    unsigned int sid = stoi(args[3]);
    std::string port = args[4];

    mocca::net::Endpoint renderConnection(m_endpoint.protocol(),
                                          m_endpoint.transport(),
                                          port);
    
    LINFO("creating render proxy for " << renderConnection);
    
    std::unique_ptr<RendererPrx> ren(new RendererPrx(std::move(renderConnection), sid));
    return ren;
}