#include "common/AbstractNode.h"

#include "commands/Reply.h"
#include "common/NetConfig.h"

#include "mocca/log/LogManager.h"

using namespace trinity;

AbstractNode::AbstractNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator, ExecutionMode executionMode)
    : m_aggregator(std::move(aggregator))
    , m_executionMode(executionMode) {}

AbstractNode::~AbstractNode() {
    join();
}

void AbstractNode::run() {
    LINFO("(node) listening... ");
    while (!isInterrupted()) {
        try {
            // receive request
            auto msgEnvelope = m_aggregator->receive(trinity::TIMEOUT_REPLY);
            if (!msgEnvelope.isNull()) {
                auto env = msgEnvelope.release();
                // LINFO("raw: " + env.message);

                auto request = Request::createFromByteArray(env.message);
                LINFO("request: " << *request);
                // handle request
                auto handler = createHandler(*request);
                auto reply = handler->execute();
                if (reply != nullptr) {
                    // send reply
                    auto serialReply = Reply::createByteArray(*reply);
                    LINFO("reply: " << *reply);
                    m_aggregator->send(mocca::net::MessageEnvelope(std::move(serialReply), env.connectionID));
                }
            }
            handleSessionErrors();
        } catch (...) {
            setException(std::current_exception());
        }
    }
}

bool AbstractNode::isLocalMachine(const std::string& machine) const {
    return m_aggregator->isLocalMachine(machine);
}

AbstractNode::ExecutionMode AbstractNode::executionMode() const {
    return m_executionMode;
}