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
    InitProcessingSessionHdl(const commands::InitProcessingSessionRequest& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    commands::InitProcessingSessionRequest m_request;
    common::IONodeProxy m_ioProxy;
};

class SetIsoValueHdl : public commands::ICommandHandler {
public:
    SetIsoValueHdl(const commands::SetIsoValueRequest& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    float m_isoValue;
    int m_sid;
};
}
}