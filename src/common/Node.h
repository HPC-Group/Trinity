#pragma once

#include <memory>
#include <vector>

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/Endpoint.h"

#include "common/NetConfig.h"
#include "common/ISession.h"
#include "commands/ICommandFactory.h"
#include "commands/Request.h"
#include "commands/Reply.h"


namespace trinity {
namespace common {

template <typename FactoryType>
class Node : public mocca::Runnable {

public:
    Node(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
        : m_aggregator(std::move(aggregator)) {}

    ~Node() {
        LINFO("(node) sessions to stop: " << m_sessions.size());
        for (auto& session : m_sessions)
            session->interrupt();
        join();
    }


    // frontend can connect, list, and init sessions
    // processing has a "joinSession(sid) which returns connection parameters

private:
    void run() {
        LINFO("(node) listening... ");

        // listening for incoming requests
        while (!isInterrupted()) {

            // receive request
            auto msgEnvelope = m_aggregator->receive(trinity::common::TIMEOUT_REPLY);

            if (!msgEnvelope.isNull()) {          // req arrived
                auto env = msgEnvelope.release(); // release value from nullable
                auto request = commands::Request::createFromByteArray(env.message);
                LINFO("request: " << *request);

                auto handler = m_factory.createHandler(*request);
                auto reply = handler->execute();
                if (reply != nullptr) {
                    auto serialReply = commands::Reply::createByteArray(*reply);
                    LINFO("reply: " << *reply);
                    m_aggregator->send(MessageEnvelope(std::move(serialReply), env.connectionID));
                }
            }
        }
    }

private:
    FactoryType m_factory;
    std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
    std::vector<std::unique_ptr<ISession>> m_sessions;
};
}
}