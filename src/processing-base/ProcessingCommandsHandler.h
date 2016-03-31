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

class EnableLightingHdl : public ICommandHandler {
public:
    EnableLightingHdl(const EnableLightingRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    EnableLightingRequest m_request;
    RenderSession* m_session;
};

class SetLightingColorsHdl : public ICommandHandler {
public:
    SetLightingColorsHdl(const SetLightingColorsRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetLightingColorsRequest m_request;
    RenderSession* m_session;
};

class GetLightingColorsHdl : public ICommandHandler {
public:
    GetLightingColorsHdl(const GetLightingColorsRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetLightingColorsRequest m_request;
    RenderSession* m_session;
};

class SetLightDirectionHdl : public ICommandHandler {
public:
    SetLightDirectionHdl(const SetLightDirectionRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetLightDirectionRequest m_request;
    RenderSession* m_session;
};

class GetLightDirectionHdl : public ICommandHandler {
public:
    GetLightDirectionHdl(const GetLightDirectionRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetLightDirectionRequest m_request;
    RenderSession* m_session;
};

class SetSampleRateModifierHdl : public ICommandHandler {
public:
    SetSampleRateModifierHdl(const SetSampleRateModifierRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetSampleRateModifierRequest m_request;
    RenderSession* m_session;
};

class GetSampleRateModifierHdl : public ICommandHandler {
public:
    GetSampleRateModifierHdl(const GetSampleRateModifierRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetSampleRateModifierRequest m_request;
    RenderSession* m_session;
};

class SetBoundingBoxModeHdl : public ICommandHandler {
public:
    SetBoundingBoxModeHdl(const SetBoundingBoxModeRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetBoundingBoxModeRequest m_request;
    RenderSession* m_session;
};

class GetBoundingBoxModeHdl : public ICommandHandler {
public:
    GetBoundingBoxModeHdl(const GetBoundingBoxModeRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBoundingBoxModeRequest m_request;
    RenderSession* m_session;
};

class SetRendererSpecialsHdl : public ICommandHandler {
public:
    SetRendererSpecialsHdl(const SetRendererSpecialsRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetRendererSpecialsRequest m_request;
    RenderSession* m_session;
};

class GetRendererSpecialsHdl : public ICommandHandler {
public:
    GetRendererSpecialsHdl(const GetRendererSpecialsRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetRendererSpecialsRequest m_request;
    RenderSession* m_session;
};

class SetBackgroundColorsHdl : public ICommandHandler {
public:
    SetBackgroundColorsHdl(const SetBackgroundColorsRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetBackgroundColorsRequest m_request;
    RenderSession* m_session;
};

class GetBackgroundColorsHdl : public ICommandHandler {
public:
    GetBackgroundColorsHdl(const GetBackgroundColorsRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBackgroundColorsRequest m_request;
    RenderSession* m_session;
};

class EnableClippingHdl : public ICommandHandler {
public:
    EnableClippingHdl(const EnableClippingRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    EnableClippingRequest m_request;
    RenderSession* m_session;
};

class SetClipVolumeHdl : public ICommandHandler {
public:
    SetClipVolumeHdl(const SetClipVolumeRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetClipVolumeRequest m_request;
    RenderSession* m_session;
};

class SetViewParametersHdl : public ICommandHandler {
public:
    SetViewParametersHdl(const SetViewParametersRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    SetViewParametersRequest m_request;
    RenderSession* m_session;
};

class RotateCameraHdl : public ICommandHandler {
public:
    RotateCameraHdl(const RotateCameraRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    RotateCameraRequest m_request;
    RenderSession* m_session;
};

class MoveCameraHdl : public ICommandHandler {
public:
    MoveCameraHdl(const MoveCameraRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    MoveCameraRequest m_request;
    RenderSession* m_session;
};

class RotateSceneHdl : public ICommandHandler {
public:
    RotateSceneHdl(const RotateSceneRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    RotateSceneRequest m_request;
    RenderSession* m_session;
};

class MoveSceneHdl : public ICommandHandler {
public:
    MoveSceneHdl(const MoveSceneRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    MoveSceneRequest m_request;
    RenderSession* m_session;
};

class RescaleSceneHdl : public ICommandHandler {
public:
    RescaleSceneHdl(const RescaleSceneRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    RescaleSceneRequest m_request;
    RenderSession* m_session;
};

class ResetCameraHdl : public ICommandHandler {
public:
    ResetCameraHdl(const ResetCameraRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    ResetCameraRequest m_request;
    RenderSession* m_session;
};

class ResetObjectHdl : public ICommandHandler {
public:
    ResetObjectHdl(const ResetObjectRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    ResetObjectRequest m_request;
    RenderSession* m_session;
};

class StartRenderingHdl : public ICommandHandler {
public:
    StartRenderingHdl(const StartRenderingRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    StartRenderingRequest m_request;
    RenderSession* m_session;
};

class StopRenderingHdl : public ICommandHandler {
public:
    StopRenderingHdl(const StopRenderingRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    StopRenderingRequest m_request;
    RenderSession* m_session;
};

class IsIdleHdl : public ICommandHandler {
public:
    IsIdleHdl(const IsIdleRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    IsIdleRequest m_request;
    RenderSession* m_session;
};

class ProceedRenderingHdl : public ICommandHandler {
public:
    ProceedRenderingHdl(const ProceedRenderingRequest& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    ProceedRenderingRequest m_request;
    RenderSession* m_session;
};

/* AUTOGEN ProcCommandHandlerHeader */
}
