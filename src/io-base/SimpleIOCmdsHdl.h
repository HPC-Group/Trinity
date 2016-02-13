#pragma once
#include "common/ICommandHandler.h"
#include "common/SimpleIOCmds.h"

namespace trinity {
namespace io {

// command-pattern like execution of trinity commands
class GetLODLevelCountHdl : public common::ICommandHandler {
    
public:
    virtual void execute();
    GetLODLevelCountHdl(common::GetLODLevelCountCmd cmd);
    ~GetLODLevelCountHdl();
    
    virtual std::unique_ptr<common::ICommand> getReturnValue();
    
private:
    common::ReplyGetLODLevelCountCmd m_reply;
    int m_sid;
};
}
}