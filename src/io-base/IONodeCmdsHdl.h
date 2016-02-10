#pragma once
#include "common/ICommandHandler.h"
#include "common/IONodeCmds.h"

namespace trinity {
namespace io {

// command-pattern like execution of trinity commands
class InitIOSessionHdl : public common::ICommandHandler {
    
public:
    virtual void execute();
    virtual std::unique_ptr<common::ICommand> getReturnValue();
    InitIOSessionHdl(common::InitIOSessionCmd cmd);
    ~InitIOSessionHdl();
    
private:

    common::ReplyInitIOSessionCmd m_reply;
    std::string m_protocol;
    int m_fileId;
};
}
}