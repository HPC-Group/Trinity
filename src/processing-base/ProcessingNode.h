#pragma once

#include "processing-base/ProcessingCommandFactory.h"

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"

#include <memory>
#include <vector>

namespace trinity {
    class ProcessingNode : public mocca::Runnable {

    public:
        ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~ProcessingNode();

        // frontend can connect, list, and init sessions
        // processing has a "joinSession(sid) which returns connection parameters

    private:
        void run();

    private:
        ProcessingNodeCommandFactory m_factory;
        std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
        //std::vector<std::unique_ptr<ISession>> m_sessions; // fixme: sessions should be stored here
    };
}
