#pragma once

#include "commands/ProcessingCommands.h"
#include "common/IProxy.h"
#include "frontend-base/RendererProxy.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "mocca/log/LogManager.h"

namespace trinity {
namespace frontend {

class ProcessingNodeProxy : public common::IProxy {

public:
    ProcessingNodeProxy(const mocca::net::Endpoint& ep)
        : common::IProxy(ep) { LINFO("(f) creating processing node proxy for " + ep.toString()); }

    std::unique_ptr<RendererProxy> initRenderer(const commands::VclType& type, int fileId,
                                                const mocca::net::Endpoint& endpoint, // io endpoint
                                                const commands::StreamingParams& streamingParams);
};
}
}