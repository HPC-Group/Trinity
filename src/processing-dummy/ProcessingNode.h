#pragma once

#include <memory>
#include <vector>

#include "mocca/net/Endpoint.h"
#include "mocca/net/ConnectionAggregator.h"
#include "RenderSession.h"
#include "mocca/base/Thread.h"



namespace trinity {
    class ProcessingNode : public mocca::Runnable {
    
public:
    
    // only frontends can connect to the processing node -> only one endpoint needed
    ProcessingNode(const mocca::net::Endpoint);
        
    ~ProcessingNode();
        

    
    
    
private:
    void run() override;
    const mocca::net::Endpoint m_endpoint;
    mocca::net::ConnectionAggregator m_aggregator;
    std::vector<std::unique_ptr<RenderSession> > m_renderSessions;
    
    std::string handleSpawnRendererCmd(std::vector<std::string>&);
    Vcl m_vcl;
    
};
}