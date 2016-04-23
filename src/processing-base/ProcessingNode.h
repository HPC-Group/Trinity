#pragma once

#include "common/AbstractNode.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/RenderSession.h"

namespace trinity {
class ProcessingNode : public AbstractNode {

public:
    ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator,
                   AbstractNode::ExecutionMode executionMode = AbstractNode::ExecutionMode::Separate,
                   CompressionMode compressionMode = CompressionMode::Compressed);
    ~ProcessingNode();

    void addSession(std::unique_ptr<RenderSession> session);
    std::vector<std::unique_ptr<RenderSession>>& getSessions();
    bool maxSessionsReached() const;

private:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;
    void handleSessionErrors() override;

private:
    ProcessingNodeCommandFactory m_factory;
    std::vector<std::unique_ptr<RenderSession>> m_sessions;
};
}
