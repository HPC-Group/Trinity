#include "RenderSession.h"
#include "mocca/base/Error.h"
#include "DummyRenderer.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"
#include "common/Commands.h"

using namespace trinity;

int RenderSession::m_basePort = 5990; // config base port for renderer here
unsigned int RenderSession::m_nextSid = 1; // sid 0 is considered invalid / dummy

RenderSession::RenderSession(const RenderType& type) {
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


std::unique_ptr<IRenderer> RenderSession::createRenderer(const RenderType& t) {
    switch (t) {
        case RenderType::DUMMY:
            return std::unique_ptr<IRenderer>(new DummyRenderer());
            break;
            
        case RenderType::GRIDLEAPER:
            throw mocca::Error("grid leaper not supported yet", __FILE__, __LINE__);
            break;
            
        default:
            throw mocca::Error("unable to create renderer: type not found", __FILE__, __LINE__);
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
    
    LINFO("render session listening at \"" + m_endpoint->toString() + "\"");
    
    try {
        
        std::unique_ptr<mocca::net::IMessageConnectionAcceptor> acceptor =
        mocca::net::ConnectionFactorySelector::bind(*m_endpoint);
        while(!m_connection && !isInterrupted()) {
            m_connection = acceptor->accept(); // auto-timeout
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("cannot bind the render session: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        auto bytepacket = m_connection->receive();
        if(!bytepacket.isEmpty()) {
            std::string cmd = bytepacket.read(bytepacket.size());
            LINFO("command arrived at renderer: " << cmd);  // print cmd
            std::vector<std::string> args = mocca::splitString<std::string>(cmd, '_');
            
            // todo check for ill-formed requests
            std::stringstream reply;
            reply << trinity::vcl::TRI_RET << "_" << args[1] << "_" << args[2] << "_" << 42;
            m_connection->send(std::move(mocca::ByteArray() << reply.str()));
            // ...
        }
    }

}