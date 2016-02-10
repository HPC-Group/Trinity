#pragma once
#include "common/ICommandHandler.h"
#include "common/ProcessingNodeCmds.h"

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
    common::StreamingParams m_params;
    std::string m_protocol;
    common::ReplyInitRendererCmd m_reply;
};
}
}