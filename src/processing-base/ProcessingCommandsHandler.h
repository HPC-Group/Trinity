#pragma once

#include "commands/ErrorCommands.h"
#include "commands/ICommandHandler.h"
#include "commands/ProcessingCommands.h"
#include "common/IONodeProxy.h"
#include "processing-base/ProcessingNode.h"
#include "processing-base/RenderSession.h"

namespace trinity {
// command-pattern like execution of trinity commands
class InitProcessingSessionHdl : public ICommandHandler {
public:
    InitProcessingSessionHdl(const InitProcessingSessionRequest& request, ProcessingNode* node);

    std::unique_ptr<Reply> execute() override;

private:
    ProcessingNode* m_node;
    InitProcessingSessionRequest m_request;
    IONodeProxy m_ioProxy;
};

class SetIsoValueHdl : public ICommandHandler {
public:
    SetIsoValueHdl(const SetIsoValueRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    RenderSession* m_session;
    float m_isoValue;
    int m_sid;
};

class InitContextHdl : public ICommandHandler {
public:
    InitContextHdl(const InitContextRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    RenderSession* m_session;
    int m_sid;
};
}