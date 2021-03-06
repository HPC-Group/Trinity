#pragma once

#include "common/Enums.h"
#include "commands/Reply.h"
#include "commands/Request.h"
#include "common/NetConfig.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

namespace trinity {
class CommandInputChannel {

public:
    CommandInputChannel(const mocca::net::Endpoint& endpoint, CompressionMode compressionMode);

    bool connect() const;
    void sendRequest(const Request& request) const;
    std::unique_ptr<Reply> getReply() const;
    mocca::net::Endpoint getEndpoint() const;

private:
    mocca::net::Endpoint m_endpoint;
    CompressionMode m_compressionMode;
    mutable std::unique_ptr<mocca::net::IMessageConnection> m_mainChannel;
};
}