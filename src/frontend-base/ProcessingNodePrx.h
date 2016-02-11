#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "RendererPrx.h"
#include "common/Commands.h"
#include "common/StreamingParams.h"
#include "common/CommandInputChannel.h"
#include "common/ProcessingNodeCmds.h"


namespace trinity {
namespace frontend {
    
class ProcessingNodePrx {
    
public:
    /// see mocca::net::Endpoint for details, supports remote and local calls
    ProcessingNodePrx(const mocca::net::Endpoint&);
    
    ~ProcessingNodePrx();
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
        
    // insert params here?
    std::unique_ptr<RendererPrx> initRenderer(const common::VclType&,
                                              int fileId,
                                              const mocca::net::Endpoint&,
                                              const common::StreamingParams&);
    
    
private:
    common::CommandInputChannel m_inputChannel;
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;
    
    std::unique_ptr<RendererPrx>
    handleInitRendererReply(const common::ReplyInitRendererCmd&,
                            const common::StreamingParams& params);
    

};
}
}