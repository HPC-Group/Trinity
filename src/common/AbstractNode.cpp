#include "common/AbstractNode.h"

#include "commands/ErrorCommands.h"
#include "commands/Reply.h"
#include "common/NetConfig.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"

using namespace trinity;

AbstractNode::AbstractNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator, ExecutionMode executionMode,
                           CompressionMode compressionMode)
    : m_aggregator(std::move(aggregator))
    , m_executionMode(executionMode)
    , m_compressionMode(compressionMode) {}

AbstractNode::~AbstractNode() {
    join();
}

void AbstractNode::run() {
    LINFO("(node) listening... ");
    while (!isInterrupted()) {
        try {
            // receive request
            auto msgEnvelope = m_aggregator->receive(trinity::TIMEOUT_DEFAULT);
            if (!msgEnvelope.isNull()) {
                auto env = msgEnvelope.release();
                auto request = Request::createFromMessage(env.message, m_compressionMode);
                // LINFO("request: " << *request);
                // handle request
                auto handler = createHandler(*request);
                std::unique_ptr<Reply> reply = nullptr;
                try {
                    reply = handler->execute();
                } catch (const TrinityError& err) {
                    ErrorCmd::ReplyParams replyParams(err.what());
                    auto errorReply = mocca::make_unique<ErrorReply>(replyParams, request->getRid(), request->getSid());
                    auto serialReply = Reply::createMessage(*errorReply, m_compressionMode);
                    m_aggregator->send(mocca::net::MessageEnvelope(std::move(serialReply), env.connectionID));
                }
                if (reply != nullptr) {
                    // send reply
                    auto serialReply = Reply::createMessage(*reply, m_compressionMode);
                    // LINFO("reply: " << *reply);
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