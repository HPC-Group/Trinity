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

RenderSession::RenderSession(const VclType& type) {
    m_sid = m_nextSid++;
    m_port = m_basePort++;
    m_renderer = createRenderer(type);
}

RenderSession::~RenderSession() {
    join();
}

void RenderSession::provideOwnEndpoint(std::unique_ptr<mocca::net::Endpoint> ep) {
    m_endpoint = std::move(ep);
}


std::unique_ptr<IRenderer> RenderSession::createRenderer(const VclType& t) {
    switch (t) {
        case VclType::DummyRenderer: {
            StreamParams params;
            params.m_resX = 1024;
            params.m_resY = 768;
            std::shared_ptr<VisStream> stream =
            std::make_shared<VisStream>(params);
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

int RenderSession::getPort() const {
    return m_port;
}


void RenderSession::run() {
    
    LINFO("(p) render session listening at \"" + m_endpoint->toString() + "\"");
    
    try {
        
        auto acceptor = mocca::net::ConnectionFactorySelector::bind(*m_endpoint);
        while(!m_connection && !isInterrupted()) {
            m_connection = acceptor->accept(); // auto-timeout
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(p) cannot bind the render session: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        auto bytepacket = m_connection->receive();
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

            m_connection->send(std::move(mocca::ByteArray() << reply));
        }
    }

}