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


IOSession::IOSession(const std::string& protocol, int fileId) :
m_sid(m_nextSid++),
m_controlPort(m_basePort++),
m_controlEndpoint(protocol, "localhost", std::to_string(m_controlPort))
{
    m_acceptor = std::move(mocca::net::ConnectionFactorySelector::bind(m_controlEndpoint));
}

unsigned int IOSession::getSid() const {
    return m_sid;
}

IIO& IOSession::getIO() {
    return *m_io;
}

IOSession::~IOSession() {
    join();
}

void IOSession::run() {
    
    LINFO("(io) IO session control at \"" + m_controlEndpoint.toString() + "\"");
    
    try {
        while(!m_controlConnection && !isInterrupted()) {
            m_controlConnection = m_acceptor->accept(); // auto-timeout
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(io) cannot bind the IO session: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        try {
            auto bytepacket = m_controlConnection->receive();
            if(!bytepacket.isEmpty()) {
                std::string cmd = bytepacket.read(bytepacket.size());
                LINFO("(io) command arrived at IO: " << cmd);  // print cmd
                
                std::stringstream requestStream, replyStream;
                requestStream << cmd;

                auto handler = m_factory.createHandler(requestStream);
                handler->execute();
                auto reply = handler->getReturnValue();
                if(reply != nullptr) {  // not tested yet
                    auto serialReply = ISerialObjectFactory::create();
                    reply->serialize(*serialReply);
                    serialReply->writeTo(replyStream);
                    LINFO("(io) reply: " << replyStream.str());
                    m_controlConnection->send(std::move(mocca::ByteArray()
                                                        << replyStream.str()));
                }
                
                
            }
        } catch (const mocca::net::NetworkError& err) {
            LWARNING("(io) remote session has gone: " << err.what());
        }

    }

}
