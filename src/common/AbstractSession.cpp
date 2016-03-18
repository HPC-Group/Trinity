#include "common/AbstractSession.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"


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

    try {
        while (!isInterrupted()) {
            auto bytepacket = m_controlConnection->receive();
            if (!bytepacket.isEmpty()) {
                auto request = Request::createFromByteArray(bytepacket);
                // LINFO("request: " << *request);

                auto handler = createHandler(*request);
                auto reply = handler->execute();
                if (reply != nullptr) { // not tested yet
                    // LINFO("reply: " << *reply);
                    auto serialReply = Reply::createByteArray(*reply);
                    m_controlConnection->send(std::move(serialReply));
                }
            }
        }
    } catch (...) {
        interrupt();
        setException(std::current_exception());
    }
}
