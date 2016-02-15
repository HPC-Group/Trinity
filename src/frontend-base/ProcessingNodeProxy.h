#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"


#include "commands/ProcessingCommands.h"
#include "common/IProxy.h"

#include "RendererProxy.h"


namespace trinity {
namespace frontend {
    
class ProcessingNodeProxy : public common::IProxy {
    
public:
    ProcessingNodeProxy(const mocca::net::Endpoint& ep) : common::IProxy(ep) {}

    std::unique_ptr<RendererProxy> initRenderer(const commands::VclType&,
                                              int fileId,
                                              const mocca::net::Endpoint&,  // io endpoint
                                              const commands::StreamingParams&);
    
    
private:
    
    std::unique_ptr<RendererProxy>
    handleInitRendererReply(const commands::ReplyInitProcessingSessionCmd&,
                            const commands::StreamingParams& params);
    

};
}
}