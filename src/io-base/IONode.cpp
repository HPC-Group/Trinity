#include "io-base/IONode.h"

#include "common/NetConfig.h"
#include "common/ISession.h"
#include "commands/Request.h"
#include "commands/Reply.h"

#include "mocca/log/LogManager.h"

using namespace trinity;

IONode::IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
    : m_aggregator(std::move(aggregator)) {}

IONode::~IONode()
{
    //LINFO("(node) sessions to stop: " << m_sessions.size());
    //for (auto& session : m_sessions)
    //    session->interrupt();
    join();
}

void IONode::run()
{
    LINFO("(node) listening... ");

    // listening for incoming requests
    while (!isInterrupted()) {

        // receive request
        auto msgEnvelope = m_aggregator->receive(trinity::TIMEOUT_REPLY);

        if (!msgEnvelope.isNull()) {          // req arrived
            auto env = msgEnvelope.release(); // release value from nullable
            auto request = Request::createFromByteArray(env.message);
            LINFO("request: " << *request);

            auto handler = m_factory.createHandler(*request);
            auto reply = handler->execute();
            if (reply != nullptr) {
                auto serialReply = Reply::createByteArray(*reply);
                LINFO("reply: " << *reply);
                m_aggregator->send(mocca::net::MessageEnvelope(std::move(serialReply), env.connectionID));
            }
        }
    }
}