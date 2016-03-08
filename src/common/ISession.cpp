#include "common/ISession.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

int ISession::m_basePort = 5990;
int ISession::m_nextSid = 1;

ISession::ISession(const std::string& protocol, std::unique_ptr<ICommandFactory> factory)
    : m_sid(m_nextSid++)
    , m_controlEndpoint(protocol, "localhost", std::to_string(m_basePort++))
    , m_factory(std::move(factory)) {
    while (!m_acceptor) {
        try {
            m_acceptor = std::move(mocca::net::ConnectionFactorySelector::bind(m_controlEndpoint));
        } catch (const mocca::net::NetworkError&) {
            LINFO("(session) cannot bind on port " << m_basePort << ", rebinding...");
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
        while (!m_controlConnection && !isInterrupted()) {
            m_controlConnection = m_acceptor->accept(); // auto-timeout
        }
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(session) cannot bind the render session: " << err.what());
        return;
    }

    // dmc: isn't this code very similar to the code in Node.cpp? maybe related to the comment in Node.cpp
    while (!isInterrupted()) {
        try {
            auto bytepacket = m_controlConnection->receive();
            if (!bytepacket.isEmpty()) {
                auto request = Request::createFromByteArray(bytepacket);
                //LINFO("request: " << *request);
                
                auto handler = m_factory->createHandler(*request);
                auto reply = handler->execute();
                if (reply != nullptr) { // not tested yet
                    //LINFO("reply: " << *reply);
                    auto serialReply = Reply::createByteArray(*reply);
                    m_controlConnection->send(std::move(serialReply));
                }
            }
        } catch (const mocca::net::NetworkError& err) {
            LWARNING("(session) interrupting because remote session has gone: " << err.what());
            interrupt();
        }
    }
}