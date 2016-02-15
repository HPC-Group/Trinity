#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"

#include "Isession.h"

#include "commands/IserialObjectFactory.h"
//#include "IserialObjectFactory.h"

using namespace trinity::common;
int ISession::m_basePort = 5990;
int ISession::m_nextSid = 1;

ISession::ISession(const std::string& protocol,
                   std::unique_ptr<commands::ICommandFactory>  factory) :
m_sid(m_nextSid++),
m_controlPort(m_basePort++),
m_controlEndpoint(protocol, "localhost", std::to_string(m_controlPort)),
m_factory(std::move(factory))
{
    while(!m_acceptor) {
        try {
            m_acceptor = std::move(mocca::net::ConnectionFactorySelector::bind(m_controlEndpoint));
            
        } catch (const mocca::net::NetworkError& err) {
            LINFO("(session) cannot bind on port " << m_basePort <<", rebinding...");
            m_controlEndpoint.port = std::to_string(m_basePort++);
        }
    }
}


ISession::~ISession() {
    join();
}

void ISession::run() {
    
    LINFO("(session) session control at \"" + m_controlEndpoint.toString() + "\"");
    
    try {
        while(!m_controlConnection && !isInterrupted()) {
            m_controlConnection = m_acceptor->accept(); // auto-timeout
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(session) cannot bind the render session: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        try {
            auto bytepacket = m_controlConnection->receive();
            if(!bytepacket.isEmpty()) {
                std::string cmd = bytepacket.read(bytepacket.size());
                LINFO("(session) command arrived at renderer: " << cmd);  // print cmd
                
                std::stringstream requestStream, replyStream;
                requestStream << cmd;

                auto handler = m_factory->createHandler(requestStream);
                handler->execute();
                auto reply = handler->getReturnValue();
                if(reply != nullptr) {  // not tested yet
                    auto serialReply = commands::ISerialObjectFactory::create();
                    reply->serialize(*serialReply);
                    serialReply->writeTo(replyStream);
                    LINFO("(session) reply: " << replyStream.str());
                    m_controlConnection->send(std::move(mocca::ByteArray()
                                                        << replyStream.str()));
                }
            }
        } catch (const mocca::net::NetworkError& err) {
            LWARNING("(session) interrupting because remote session has gone: " << err.what());
            interrupt();
        }
    }
}


void SessionManager::addSession(std::unique_ptr<ISession> session) {
    m_sessions.push_back(std::move(session));
}

void SessionManager::endSession(int sid) {
    for(auto& session : m_sessions) {
        if(session->getSid() == sid)
            session->interrupt();
    }
    LWARNING("(io) can't end session with id " + std::to_string(sid) + "(does not exist)");
}

void SessionManager::endAllSessions() {
    LINFO("(io) IOers to stop: " << m_sessions.size());
    for (auto& session : m_sessions)
        session->interrupt();
}

ISession& SessionManager::getSession(int sid) {
    for(auto& session : m_sessions) {
        if(session->getSid() == sid)
            return *session;
    }
    throw mocca::Error("no session with id " + std::to_string(sid), __FILE__, __LINE__);
}