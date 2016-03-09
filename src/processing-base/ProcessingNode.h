#pragma once

#include "common/AbstractNode.h"
#include "processing-base/ProcessingCommandFactory.h"

namespace trinity {
    class ProcessingNode : public AbstractNode {

    public:
        ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~ProcessingNode();

    private:
        std::unique_ptr<ICommandHandler> createHandler(const Request& request) const override;

    private:
        ProcessingNodeCommandFactory m_factory;
        //std::vector<std::unique_ptr<ISession>> m_sessions; // fixme: sessions should be stored here
    };
}
