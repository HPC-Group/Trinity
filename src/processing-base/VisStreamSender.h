#pragma once
#include <memory>

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/base/Thread.h"
#include "common/VisStream.h"

// subject of refactoring
namespace trinity {
namespace processing {

// todo sender and receiver do share much code for now
// however, this will change when videocodec will be added
class VisStreamSender : public mocca::Runnable {
    
public:
    VisStreamSender(const mocca::net::Endpoint, std::shared_ptr<common::VisStream>);
    ~VisStreamSender();
    
    void startStreaming();
    void endStreaming();
    
private:
    
    std::shared_ptr<common::VisStream> m_visStream;
    void run() override;
    
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_connection;
};
}
}