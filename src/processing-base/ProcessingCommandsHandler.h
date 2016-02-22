#pragma once
#include "commands/ErrorCommands.h"
#include "commands/ICommandHandler.h"
#include "commands/ProcessingCommands.h"
#include "common/IONodeProxy.h"

namespace trinity {
namespace processing {

// command-pattern like execution of trinity commands
class InitProcessingSessionHdl : public commands::ICommandHandler {

public:
    virtual void execute();
    virtual std::unique_ptr<commands::ICommand> getReturnValue();
    InitProcessingSessionHdl(commands::InitProcessingSessionCmd cmd);
    ~InitProcessingSessionHdl();

private:
    commands::InitProcessingSessionCmd m_cmd;
    commands::ReplyInitProcessingSessionCmd m_reply;
    std::unique_ptr<commands::ErrorCmd> m_errorReply;
    common::IONodeProxy m_ioProxy;
};

class SetIsoValueHdl : public commands::ICommandHandler {

public:
    virtual void execute();
    SetIsoValueHdl(commands::SetIsoValueCmd cmd);
    ~SetIsoValueHdl();


private:
    float m_isoValue;
    int m_sid;
};
}
}