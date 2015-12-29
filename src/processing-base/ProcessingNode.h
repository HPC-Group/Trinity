#pragma once

#include <memory>
#include <vector>

#include "mocca/net/Endpoint.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/base/Thread.h"

#include "RenderSession.h"



namespace trinity {
namespace processing {

// todo: refactor such that the aggregator has all connection entities
// node waiting for frontends to connect. Spawns RenderSessions
class ProcessingNode : public mocca::Runnable {
    
public:
    
    // only frontends can connect to the processing node -> only one endpoint needed
    ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator>);
    ~ProcessingNode();
        
 
private:
    void run() override;
    std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
    std::vector<std::unique_ptr<RenderSession> > m_renderSessions;
    
    std::string handleInitRendererCmd(std::vector<std::string>&);
    common::Vcl m_vcl;
    
};
}
}