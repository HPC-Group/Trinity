#pragma once
#include "commands/ErrorCommands.h"
#include "commands/ICommandHandler.h"
#include "commands/ProcessingCommands.h"
#include "common/IONodeProxy.h"

namespace trinity {
// command-pattern like execution of trinity commands
class InitProcessingSessionHdl : public ICommandHandler {
public:
    InitProcessingSessionHdl(const InitProcessingSessionRequest& request);

    std::unique_ptr<Reply> execute() override;

private:
    InitProcessingSessionRequest m_request;
    IONodeProxy m_ioProxy;
};

class SetIsoValueHdl : public ICommandHandler {
public:
    SetIsoValueHdl(const SetIsoValueRequest& request);

    std::unique_ptr<Reply> execute() override;

private:
    float m_isoValue;
    int m_sid;
};

class InitContextHdl : public ICommandHandler {
public:
    InitContextHdl(const InitContextRequest& request);

    std::unique_ptr<Reply> execute() override;

private:
    int m_sid;
};
}