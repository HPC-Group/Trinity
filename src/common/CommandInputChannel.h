#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"


#include "ICommand.h"
#include "NetConfig.h"

namespace trinity {
namespace common {

class CommandInputChannel {
    
public:
    CommandInputChannel(const mocca::net::Endpoint&);
    ~CommandInputChannel();
    bool connect();
    void sendCommand(const ICommand& cmd);
    std::unique_ptr<ISerialObject> getReply(const std::chrono::milliseconds& = TIMEOUT_REPLY);
    const mocca::net::Endpoint& getEndpoint() const;
    
private:
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
};
}
}