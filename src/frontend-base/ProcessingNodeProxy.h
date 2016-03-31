#pragma once

#include "commands/ProcessingCommands.h"
#include "frontend-base/RendererProxy.h"
#include "commands/CommandInputChannel.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "mocca/log/LogManager.h"

namespace trinity {
class ProcessingNodeProxy  {

public:
    ProcessingNodeProxy(const mocca::net::Endpoint& ep);

    std::unique_ptr<RendererProxy> initRenderer(const VclType& type, const std::string& fileId,
                                                const mocca::net::Endpoint& ioEndpoint,
                                                const StreamingParams& streamingParams);

private:
    CommandInputChannel m_inputChannel;
};
}