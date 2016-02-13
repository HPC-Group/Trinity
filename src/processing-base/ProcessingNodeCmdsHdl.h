#pragma once
#include "common/ICommandHandler.h"
#include "common/ProcessingNodeCmds.h"

#include "IONodeProcessingPrx.h"

namespace trinity {
namespace processing {

// command-pattern like execution of trinity commands
class InitRendererHdl : public common::ICommandHandler {
    
public:
    virtual void execute();
    virtual std::unique_ptr<common::ICommand> getReturnValue();
    InitRendererHdl(common::InitRendererCmd cmd);
    ~InitRendererHdl();
    
private:
    common::InitRendererCmd m_cmd;
    common::ReplyInitRendererCmd m_reply;
    IONodeProcessingPrx m_ioProxy;
};
}
}