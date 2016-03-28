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
    SetIsoValueRequest m_request;
};

class InitContextHdl : public ICommandHandler {
public:
    InitContextHdl(const InitContextRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    RenderSession* m_session;
    int m_sid;
};

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

class SetActiveModalityHdl : public ICommandHandler {
public:
    SetActiveModalityHdl(const SetActiveModalityRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetActiveModalityRequest m_request;
    RenderSession* m_session;
};

class GetActiveModalityHdl : public ICommandHandler {
public:
    GetActiveModalityHdl(const GetActiveModalityRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetActiveModalityRequest m_request;
    RenderSession* m_session;
};

class SetActiveTimestepHdl : public ICommandHandler {
public:
    SetActiveTimestepHdl(const SetActiveTimestepRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetActiveTimestepRequest m_request;
    RenderSession* m_session;
};

class GetActiveTimestepHdl : public ICommandHandler {
public:
    GetActiveTimestepHdl(const GetActiveTimestepRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetActiveTimestepRequest m_request;
    RenderSession* m_session;
};

class GetModalityCountProcHdl : public ICommandHandler {
public:
    GetModalityCountProcHdl(const GetModalityCountProcRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetModalityCountProcRequest m_request;
    RenderSession* m_session;
};

class GetTimestepCountProcHdl : public ICommandHandler {
public:
    GetTimestepCountProcHdl(const GetTimestepCountProcRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetTimestepCountProcRequest m_request;
    RenderSession* m_session;
};

class Set1DTransferFunctionHdl : public ICommandHandler {
public:
    Set1DTransferFunctionHdl(const Set1DTransferFunctionRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    Set1DTransferFunctionRequest m_request;
    RenderSession* m_session;
};

class GetDefault1DTransferFunctionProcHdl : public ICommandHandler {
public:
    GetDefault1DTransferFunctionProcHdl(const GetDefault1DTransferFunctionProcRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDefault1DTransferFunctionProcRequest m_request;
    RenderSession* m_session;
};

class GetDefault1DTransferFunctionCountProcHdl : public ICommandHandler {
public:
    GetDefault1DTransferFunctionCountProcHdl(const GetDefault1DTransferFunctionCountProcRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDefault1DTransferFunctionCountProcRequest m_request;
    RenderSession* m_session;
};

class Get1DHistogramProcHdl : public ICommandHandler {
public:
    Get1DHistogramProcHdl(const Get1DHistogramProcRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    Get1DHistogramProcRequest m_request;
    RenderSession* m_session;
};

class GetIsoValueHdl : public ICommandHandler {
public:
    GetIsoValueHdl(const GetIsoValueRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetIsoValueRequest m_request;
    RenderSession* m_session;
};

class SetIsosurfaceColorHdl : public ICommandHandler {
public:
    SetIsosurfaceColorHdl(const SetIsosurfaceColorRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetIsosurfaceColorRequest m_request;
    RenderSession* m_session;
};

class GetIsosurfaceColorHdl : public ICommandHandler {
public:
    GetIsosurfaceColorHdl(const GetIsosurfaceColorRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetIsosurfaceColorRequest m_request;
    RenderSession* m_session;
};

class GetRangeProcHdl : public ICommandHandler {
public:
    GetRangeProcHdl(const GetRangeProcRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetRangeProcRequest m_request;
    RenderSession* m_session;
};

class SetClearViewPositionHdl : public ICommandHandler {
public:
    SetClearViewPositionHdl(const SetClearViewPositionRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetClearViewPositionRequest m_request;
    RenderSession* m_session;
};

class SetClearViewRadiusHdl : public ICommandHandler {
public:
    SetClearViewRadiusHdl(const SetClearViewRadiusRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetClearViewRadiusRequest m_request;
    RenderSession* m_session;
};

class SetClearBorderSizeHdl : public ICommandHandler {
public:
    SetClearBorderSizeHdl(const SetClearBorderSizeRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetClearBorderSizeRequest m_request;
    RenderSession* m_session;
};

/* AUTOGEN ProcCommandHandlerHeader */
}
