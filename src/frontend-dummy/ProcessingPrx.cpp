#include <sstream>

#include "ProcessingPrx.h"
#include "common/MuiError.h"
#include <thread>

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
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
        m_mainChannel = mocca::net::ConnectionFactorySelector::connect(m_endpoint);
        
    } catch (const mocca::net::ConnectFailedError&) {
        LWARNING("(f) no connection to processing  at \"" << m_endpoint << "\": ");
        return false;
    }

    std::string cmd = m_vcl.assembleInitConnection(0, m_ridGen.nextID());
    m_mainChannel->send(std::move(mocca::ByteArray()<< cmd));
    
    LINFO("(f) successfully connected to processing at \"" << m_endpoint << "\": ");
    return true;
}


std::unique_ptr<RendererPrx> ProcessingPrx::initRenderer(const VclType& type) {
    
    std::string cmd = m_vcl.assembleInitRenderer(0, m_ridGen.nextID(), type);
    m_mainChannel->send(std::move(mocca::ByteArray()<< cmd));
    
    auto byteArray = m_mainChannel->receive(trinity::config::TIMEOUT_REPLY);
    
    if(byteArray.isEmpty()) {
        throw mocca::Error("cannot spawn renderer, no reply arrived", __FILE__, __LINE__);
    }
                           
    std::string rep = byteArray.read(byteArray.size());
    LINFO("(f) cmd : " << cmd << "; reply: " << rep);
    std::vector<std::string> args = mocca::splitString<std::string>(rep, '_');
    
    
    if (!m_vcl.isSoundCommand(args)) {
        throw mocca::Error("spawn renderer: result has wrong format", __FILE__, __LINE__);
    }
    
    VclType t = m_vcl.toType(args[0]);

    switch(t) {
        case VclType::TrinityError: {
            throw mocca::Error("cannot spawn renderer,  code: " + args[3],
                                                       __FILE__, __LINE__);
            break;
        }
            
        case VclType::TrinityReturn: {
            unsigned int sid = stoi(args[3]);
            std::string port = args[4];
            
            mocca::net::Endpoint renderConnection(m_endpoint.protocol(),
                                                  port);
            
            LINFO("(f) creating render proxy for " << renderConnection);
            
            std::unique_ptr<RendererPrx>
            ren(new RendererPrx(std::move(renderConnection), sid));
            return ren;
        }
            
        default:
            throw mocca::Error("unknown type: " + m_vcl.toString(t),
                               __FILE__, __LINE__);
    }
}