#pragma once

#include <memory>
#include <vector>

#include "mocca/net/Endpoint.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/base/Thread.h"

#include "commands/ICommandFactory.h"
#include "ISession.h"


namespace trinity {
namespace common {

// todo: IO and processing nodes feel like they are the same -> refactor
class Node : public mocca::Runnable {
    
public:
    
    Node(std::unique_ptr<mocca::net::ConnectionAggregator>,
          std::unique_ptr<commands::ICommandFactory>);
    ~Node();
    
    
    // frontend can connect, list, and init sessions
    // processing has a "joinSession(sid) which returns connection parameters
 
private:
    std::unique_ptr<commands::ICommandFactory> m_factory;
    
    void run() override;
    std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
    std::vector<std::unique_ptr<ISession> > m_sessions;
};
}
}