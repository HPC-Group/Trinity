#include <sstream>
#include <thread>

#include "ProcessingNodePrx.h"
#include "mocca/base/Error.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"
#include "common/NetConfig.h"


using namespace trinity::frontend;
using namespace trinity::common;

static std::chrono::milliseconds receiveTimeout(50);

ProcessingNodePrx::~ProcessingNodePrx() {}

ProcessingNodePrx::ProcessingNodePrx(mocca::net::Endpoint endpoint) :
m_endpoint(endpoint) {}


bool ProcessingNodePrx::connect() {
    
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


std::unique_ptr<RendererPrx> ProcessingNodePrx::initRenderer(const VclType& type) {
    
    std::string cmd = m_vcl.assembleInitRenderer(0, m_ridGen.nextID(), type);
    m_mainChannel->send(std::move(mocca::ByteArray()<< cmd));
    
    auto byteArray = m_mainChannel->receive(trinity::common::TIMEOUT_REPLY);
    
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
            StreamParams params;
            params.m_resX = 1024;
            params.m_resY = 768;
            std::shared_ptr<VisStream> stream =
            std::make_shared<VisStream>(params);
            
            LINFO("(f) creating render proxy for " << renderConnection);
            
            std::unique_ptr<RendererPrx>
            ren(new RendererPrx(stream, std::move(renderConnection), sid));
            return ren;
        }
            
        default:
            throw mocca::Error("unknown type: " + m_vcl.toString(t),
                               __FILE__, __LINE__);
    }
}