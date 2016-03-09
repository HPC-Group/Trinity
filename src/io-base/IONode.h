#pragma once

#include "common/AbstractNode.h"
#include "io-base/IOCommandFactory.h"

namespace trinity {
    class IONode : public AbstractNode {

    public:
        IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~IONode();

    private:
        std::unique_ptr<ICommandHandler> createHandler(const Request& request) const override;

    private:
        IONodeCommandFactory m_factory;
        //std::vector<std::unique_ptr<ISession>> m_sessions; // fixme: sessions should be stored here
    };
}
