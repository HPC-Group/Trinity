#pragma once

#include "commands/Reply.h"
#include "commands/Request.h"
#include "common/NetConfig.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

namespace trinity {
namespace commands {

class CommandInputChannel {

public:
    CommandInputChannel(const mocca::net::Endpoint& endpoint);

    bool connect();
    void sendRequest(const Request& request);
    std::unique_ptr<Reply> getReply(const std::chrono::milliseconds& = common::TIMEOUT_REPLY);
    mocca::net::Endpoint getEndpoint() const;

private:
    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
};
}
}