#pragma once
#include "commands/ICommandHandler.h"
#include "commands/IOCommands.h"

namespace trinity {
namespace io {

// command-pattern like execution of trinity commands
class InitIOSessionHdl : public commands::ICommandHandler {

public:
    virtual void execute();
    virtual std::unique_ptr<commands::ICommand> getReturnValue();
    InitIOSessionHdl(commands::InitIOSessionCmd cmd);
    ~InitIOSessionHdl() = default;

private:
    commands::ReplyInitIOSessionCmd m_reply;
    std::string m_protocol;
    int m_fileId;
};


class GetLODLevelCountHdl : public commands::ICommandHandler {

public:
    virtual void execute();
    GetLODLevelCountHdl(commands::GetLODLevelCountCmd cmd);
    ~GetLODLevelCountHdl() = default;

    virtual std::unique_ptr<commands::ICommand> getReturnValue();

private:
    commands::ReplyGetLODLevelCountCmd m_reply;
    int m_sid;
};
}
}