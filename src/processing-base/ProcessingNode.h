#pragma once

#include "common/AbstractNode.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/RenderSession.h"

namespace trinity {
    class ProcessingNode : public AbstractNode {

    public:
        ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~ProcessingNode();

        void addSession(std::unique_ptr<RenderSession> session);
        std::vector<std::unique_ptr<RenderSession>>& getSessions();

    private:
        std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;

    private:
        ProcessingNodeCommandFactory m_factory;
        std::vector<std::unique_ptr<RenderSession>> m_sessions;
    };
}
