#pragma once

#include "io-base/IOCommandFactory.h"

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"

#include <memory>
#include <vector>

namespace trinity {
    class IONode : public mocca::Runnable {

    public:
        IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~IONode();

        // frontend can connect, list, and init sessions
        // processing has a "joinSession(sid) which returns connection parameters

    private:
        void run();

    private:
        IONodeCommandFactory m_factory;
        std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
        //std::vector<std::unique_ptr<ISession>> m_sessions; // fixme: sessions should be stored here
    };
}
