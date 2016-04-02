#include "common/AbstractSession.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/message/NewLoopbackConnection.h"

using namespace mocca::net;
using namespace trinity;

AbstractSession::AbstractSession(const std::string& protocol)
    : m_sid(IDGenerator::nextID())
    // FIXME dmc: "localhost" should be "*", but then the tests fail -> find out why!
    , m_acceptor(ConnectionFactorySelector::bind(Endpoint(protocol, "localhost", Endpoint::autoPort()))) {}

AbstractSession::~AbstractSession() {
    LINFO("(session) joining session...");
    join();
    LINFO("(session) session joined");
}

void AbstractSession::run() {
    LINFO("(session) session control at \"" << *m_acceptor->localEndpoint() << "\"");

    try {
        while (!m_controlConnection && !isInterrupted()) {
            m_controlConnection = m_acceptor->accept(); // auto-timeout
        }
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(session) cannot bind the render session: " << err.what());
        return;
    }

    if (m_controlConnection) {
        LDEBUG("Initiated session of type " << typeid(*this).name() << " with connection ID " << *m_controlConnection->connectionID());
    }

    performThreadSpecificInit();
    try {
        while (!isInterrupted()) {
            if (auto loopback = dynamic_cast<mocca::net::NewLoopbackConnection*>(m_controlConnection.get())) {
                auto message = loopback->receiveNew();
                if (!message.isEmpty()) {
                    auto request = Request::createFromMessage(message);
                    // LINFO("request: " << *request);
                    auto handler = createHandler(*request);
                    auto reply = handler->execute();
                    if (reply != nullptr) { // not tested yet
                        auto serialReply = Reply::createMessage(*reply);
                        loopback->sendNew(std::move(serialReply));
                    }
                }
            } else {
                auto bytepacket = m_controlConnection->receive();
                if (!bytepacket.isEmpty()) {
                    auto request = Request::createFromByteArray(bytepacket);
                    // LINFO("request: " << *request);
                    auto handler = createHandler(*request);
                    auto reply = handler->execute();
                    if (reply != nullptr) { // not tested yet
                        auto serialReply = Reply::createByteArray(*reply);
                        m_controlConnection->send(std::move(serialReply));
                    }
                }
            }
        }
    } catch (...) {
        interrupt();
        performThreadSpecificTeardown();
        setException(std::current_exception());
    }
}
