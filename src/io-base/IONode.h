#pragma once

#include "common/AbstractNode.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOSession.h"

namespace trinity {
    class IONode : public AbstractNode {

    public:
        IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~IONode();

        void addSession(std::unique_ptr<IOSession> session);

    private:
        std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;

    private:
        IONodeCommandFactory m_factory;
        std::vector<std::unique_ptr<IOSession>> m_sessions;
    };
}
