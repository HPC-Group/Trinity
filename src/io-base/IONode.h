#pragma once

#include <memory>
#include <vector>

#include "mocca/net/Endpoint.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/base/Thread.h"

#include "IOSession.h"
#include "IOCommandFactory.h"



namespace trinity {
namespace io {

// todo: IO and processing nodes feel like they are the same -> refactor
class IONode : public mocca::Runnable {
    
public:
    
    IONode(std::unique_ptr<mocca::net::ConnectionAggregator>);
    ~IONode();
    
    
    // frontend can connect, list, and init sessions
    // processing has a "joinSession(sid) which returns connection parameters
 
private:
    common::Vcl m_vcl;
    IOCommandFactory m_factory;
    
    void run() override;
    std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
    std::vector<std::unique_ptr<IOSession> > m_ioSessions;
};
}
}