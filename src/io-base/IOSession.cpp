#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"

#include "common/Commands.h"
#include "common/ISerialObjectFactory.h"

#include "DummyIO.h"
#include "IOSession.h"

using namespace trinity::io;
using namespace trinity::common;

int IOSession::m_basePort = 6990; // config base port for IOer here
unsigned int IOSession::m_nextSid = 1; // sid 0 is considered invalid / dummy




unsigned int IOSession::getSid() const {
    return m_sid;
}

IIO& IOSession::getIO() {
    return *m_io;
}


/*
IOSession::IOSession(const VclType& IOerType,
                             const StreamingParams& params,
                             const std::string& protocol) :
m_sid(m_nextSid++),
m_controlPort(m_basePort++),
m_visPort(m_basePort++),
m_controlEndpoint(protocol, "localhost", std::to_string(m_controlPort)),
m_visSender(mocca::net::Endpoint(protocol, "localhost", std::to_string(m_visPort)),
            std::make_shared<VisStream>(params))
{

    m_IOer = createIOer(IOerType, params);
    m_acceptor = std::move(mocca::net::ConnectionFactorySelector::bind(m_controlEndpoint));
    m_visSender.startStreaming();
}

IOSession::~IOSession() {
    m_visSender.endStreaming();
    join();
}



std::unique_ptr<IIOer> IOSession::createIOer(const VclType& IOerType,
                                                         const StreamingParams& params) {
    switch (IOerType) {
        case VclType::DummyIOer: {

            std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(params);
            return std::unique_ptr<IIOer>(new DummyIOer(stream));
            break;
        }
            
        case VclType::GridLeaper:
            throw mocca::Error("grid leaper not supported yet", __FILE__, __LINE__);
            break;
            
        default:
            throw mocca::Error("can't create IOer: no such type", __FILE__, __LINE__);
            break;
    }
}


unsigned int IOSession::getSid() const {
    return m_sid;
}

int IOSession::getControlPort() const {
    return m_controlPort;
}

int IOSession::getVisPort() const {
    return m_visPort;
}

IIOer& IOSession::getIOer() {
    return *m_IOer;
}


void IOSession::run() {
    
    LINFO("(p) IO session control at \"" + m_controlEndpoint.toString() + "\"");
    
    try {
        while(!m_controlConnection && !isInterrupted()) {
            m_controlConnection = m_acceptor->accept(); // auto-timeout
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(p) cannot bind the IO session: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        try {
            auto bytepacket = m_controlConnection->receive();
            if(!bytepacket.isEmpty()) {
                std::string cmd = bytepacket.read(bytepacket.size());
                LINFO("(p) command arrived at IOer: " << cmd);  // print cmd
                
                std::stringstream requestStream, replyStream;
                requestStream << cmd;

                auto handler = m_factory.createHandler(requestStream);
                handler->execute();
                auto reply = handler->getReturnValue();
                if(reply != nullptr) {  // not tested yet
                    auto serialReply = ISerialObjectFactory::create();
                    reply->serialize(*serialReply);
                    serialReply->writeTo(replyStream);
                    LINFO("(p) reply: " << replyStream.str());
                    m_controlConnection->send(std::move(mocca::ByteArray()
                                                        << replyStream.str()));
                }
                
                
            }
        } catch (const mocca::net::NetworkError& err) {
            LWARNING("(p) remote session has gone: " << err.what());
        }

    }

}
*/