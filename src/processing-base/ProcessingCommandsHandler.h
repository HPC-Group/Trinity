#pragma once

#include "commands/ErrorCommands.h"
#include "commands/ICommandHandler.h"
#include "commands/ProcessingCommands.h"
#include "commands/Request.h"
#include "commands/Reply.h"
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

#define PYTHON_MAGIC


class ZoomCameraHdl : public ICommandHandler {
public:
    ZoomCameraHdl(const ZoomCameraRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    ZoomCameraRequest m_request;
    RenderSession* m_session;
};

class SetRenderModeHdl : public ICommandHandler {
public:
    SetRenderModeHdl(const SetRenderModeRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetRenderModeRequest m_request;
    RenderSession* m_session;
};

class SupportsRenderModeHdl : public ICommandHandler {
public:
    SupportsRenderModeHdl(const SupportsRenderModeRequest& request, RenderSession*);

    std::unique_ptr<Reply> execute() override;

private:
    SupportsRenderModeRequest m_request;
    RenderSession* m_session;
};

#undef PYTHON_MAGIC
}
