#include "RenderSession.h"
#include "mocca/base/Error.h"
#include "DummyRenderer.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"
#include "common/Commands.h"

using namespace trinity::processing;
using namespace trinity::common;

int RenderSession::m_basePort = 5990; // config base port for renderer here
unsigned int RenderSession::m_nextSid = 1; // sid 0 is considered invalid / dummy

RenderSession::RenderSession(const VclType& rendererType,
                             const StreamParams& params,
                             const std::string& protocol) :
m_sid(m_nextSid++),
m_controlPort(m_basePort++),
m_visPort(m_basePort++),
m_controlEndpoint(protocol, "localhost", std::to_string(m_controlPort)),
m_visSender(mocca::net::Endpoint(protocol, "localhost", std::to_string(m_visPort)),
            std::make_shared<VisStream>(params))
{
    m_renderer = createRenderer(rendererType, params);
    m_visSender.startStreaming();
}

RenderSession::RenderSession(const VclType& rendererType,
                             const StreamingParams& params,
                             const std::string& protocol) :
m_sid(m_nextSid++),
m_controlPort(m_basePort++),
m_visPort(m_basePort++),
m_controlEndpoint(protocol, "localhost", std::to_string(m_controlPort)),
m_visSender(mocca::net::Endpoint(protocol, "localhost", std::to_string(m_visPort)),
            std::make_shared<VisStream>(params))
{
    StreamParams p;
    p.m_resX = params.getResX();
    p.m_resY = params.getResY();
    m_renderer = createRenderer(rendererType, p);
    m_visSender.startStreaming();
}

RenderSession::~RenderSession() {
    m_visSender.endStreaming();
    join();
}



std::unique_ptr<IRenderer> RenderSession::createRenderer(const VclType& rendererType,
                                                         const StreamParams& params) {
    switch (rendererType) {
        case VclType::DummyRenderer: {

            std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(params);
            return std::unique_ptr<IRenderer>(new DummyRenderer(stream));
            break;
        }
            
        case VclType::GridLeaper:
            throw mocca::Error("grid leaper not supported yet", __FILE__, __LINE__);
            break;
            
        default:
            throw mocca::Error("can't create renderer: no such type", __FILE__, __LINE__);
            break;
    }
}


unsigned int RenderSession::getSid() const {
    return m_sid;
}

int RenderSession::getControlPort() const {
    return m_controlPort;
}

int RenderSession::getVisPort() const {
    return m_visPort;
}


void RenderSession::run() {
    
    LINFO("(p) render session control at \"" + m_controlEndpoint.toString() + "\"");
    
    try {
        
        auto acceptor = mocca::net::ConnectionFactorySelector::bind(m_controlEndpoint);
        while(!m_controlConnection && !isInterrupted()) {
            m_controlConnection = acceptor->accept(); // auto-timeout
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(p) cannot bind the render session: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        try {
            auto bytepacket = m_controlConnection->receive();
            if(!bytepacket.isEmpty()) {
                std::string cmd = bytepacket.read(bytepacket.size());
                LINFO("(p) command arrived at renderer: " << cmd);  // print cmd
                std::vector<std::string> args = mocca::splitString<std::string>(cmd, '_');
                
                std::string reply;
                
                if (!m_vcl.isSoundCommand(args)) {
                    reply = m_vcl.assembleError(0, 0, 1);
                }
                
                VclType t = m_vcl.toType(args[0]);
                
                // we don't have any commands yet
                switch(t) {
                    default:
                        reply = m_vcl.assembleError(0, stoi(args[2]), 2);
                }
                
                m_controlConnection->send(std::move(mocca::ByteArray() << reply));
            }
        } catch (const mocca::net::NetworkError& err) {
            LWARNING("(p) remote session has gone: " << err.what());
        }

    }

}