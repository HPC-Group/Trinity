#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"

#include <memory>
#include <vector>

namespace trinity {
    class AbstractNode : public mocca::Runnable {

    public:
        AbstractNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~AbstractNode();

    private:
        void run();
        virtual std::unique_ptr<ICommandHandler> createHandler(const Request& request) = 0;

    private:
        std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
    };
}
