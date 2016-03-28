#include "processing-base/ProcessingCommandFactory.h"

#include "commands/ProcessingCommands.h"
#include "common/TrinityError.h"
#include "processing-base/ProcessingCommandsHandler.h"
#include "processing-base/ProcessingNode.h"
#include "processing-base/RenderSession.h"

#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"

#include <iostream>

using namespace trinity;

std::unique_ptr<ICommandHandler> ProcessingNodeCommandFactory::createHandler(const Request& request, ProcessingNode* node) const {
    VclType type = request.getType();

    switch (type) {
    case VclType::InitRenderer:
        return mocca::make_unique<InitProcessingSessionHdl>(static_cast<const InitProcessingSessionRequest&>(request), node);
        break;

    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}

std::unique_ptr<ICommandHandler> ProcessingSessionCommandFactory::createHandler(const Request& request, RenderSession* session) const {
    VclType type = request.getType();

    switch (type) {
    case VclType::SetIsoValue:
        return mocca::make_unique<SetIsoValueHdl>(static_cast<const SetIsoValueRequest&>(request), session);
        break;
    case VclType::InitContext:
        return mocca::make_unique<InitContextHdl>(static_cast<const InitContextRequest&>(request), session);
        break;
    case VclType::ZoomCamera:
        return mocca::make_unique<ZoomCameraHdl>(static_cast<const ZoomCameraRequest&>(request), session);
        break;
    case VclType::SetRenderMode:
        return mocca::make_unique<SetRenderModeHdl>(static_cast<const SetRenderModeRequest&>(request), session);
        break;
    case VclType::SupportsRenderMode:
        return mocca::make_unique<SupportsRenderModeHdl>(static_cast<const SupportsRenderModeRequest&>(request), session);
        break;
    case VclType::SetActiveModality:
        return mocca::make_unique<SetActiveModalityHdl>(static_cast<const SetActiveModalityRequest&>(request), session);
        break;
    case VclType::GetActiveModality:
        return mocca::make_unique<GetActiveModalityHdl>(static_cast<const GetActiveModalityRequest&>(request), session);
        break;
    case VclType::SetActiveTimestep:
        return mocca::make_unique<SetActiveTimestepHdl>(static_cast<const SetActiveTimestepRequest&>(request), session);
        break;
    case VclType::GetActiveTimestep:
        return mocca::make_unique<GetActiveTimestepHdl>(static_cast<const GetActiveTimestepRequest&>(request), session);
        break;
    case VclType::GetModalityCountProc:
        return mocca::make_unique<GetModalityCountProcHdl>(static_cast<const GetModalityCountProcRequest&>(request), session);
        break;
    case VclType::GetTimestepCountProc:
        return mocca::make_unique<GetTimestepCountProcHdl>(static_cast<const GetTimestepCountProcRequest&>(request), session);
        break;
    case VclType::Set1DTransferFunction:
        return mocca::make_unique<Set1DTransferFunctionHdl>(static_cast<const Set1DTransferFunctionRequest&>(request), session);
        break;
    case VclType::GetDefault1DTransferFunctionProc:
        return mocca::make_unique<GetDefault1DTransferFunctionProcHdl>(static_cast<const GetDefault1DTransferFunctionProcRequest&>(request),
                                                                       session);
        break;
    case VclType::GetDefault1DTransferFunctionCountProc:
        return mocca::make_unique<GetDefault1DTransferFunctionCountProcHdl>(
            static_cast<const GetDefault1DTransferFunctionCountProcRequest&>(request), session);
        break;
    case VclType::Get1DHistogramProc:
        return mocca::make_unique<Get1DHistogramProcHdl>(static_cast<const Get1DHistogramProcRequest&>(request), session);
        break;
    case VclType::GetIsoValue:
        return mocca::make_unique<GetIsoValueHdl>(static_cast<const GetIsoValueRequest&>(request), session);
        break;
    case VclType::SetIsosurfaceColor:
        return mocca::make_unique<SetIsosurfaceColorHdl>(static_cast<const SetIsosurfaceColorRequest&>(request), session);
        break;
    case VclType::GetIsosurfaceColor:
        return mocca::make_unique<GetIsosurfaceColorHdl>(static_cast<const GetIsosurfaceColorRequest&>(request), session);
        break;
    case VclType::GetRangeProc:
        return mocca::make_unique<GetRangeProcHdl>(static_cast<const GetRangeProcRequest&>(request), session);
        break;
    case VclType::SetClearViewPosition:
        return mocca::make_unique<SetClearViewPositionHdl>(static_cast<const SetClearViewPositionRequest&>(request), session);
        break;
    case VclType::SetClearViewRadius:
        return mocca::make_unique<SetClearViewRadiusHdl>(static_cast<const SetClearViewRadiusRequest&>(request), session);
        break;
    case VclType::SetClearBorderSize:
        return mocca::make_unique<SetClearBorderSizeHdl>(static_cast<const SetClearBorderSizeRequest&>(request), session);
        break;
    case VclType::EnableLighting:
        return mocca::make_unique<EnableLightingHdl>(static_cast<const EnableLightingRequest&>(request), session);
        break;
    case VclType::SetLightingColors:
        return mocca::make_unique<SetLightingColorsHdl>(static_cast<const SetLightingColorsRequest&>(request), session);
        break;
    case VclType::GetLightingColors:
        return mocca::make_unique<GetLightingColorsHdl>(static_cast<const GetLightingColorsRequest&>(request), session);
        break;
    case VclType::SetLightDirection:
        return mocca::make_unique<SetLightDirectionHdl>(static_cast<const SetLightDirectionRequest&>(request), session);
        break;
    case VclType::GetLightDirection:
        return mocca::make_unique<GetLightDirectionHdl>(static_cast<const GetLightDirectionRequest&>(request), session);
        break;
    case VclType::SetSampleRateModifier:
        return mocca::make_unique<SetSampleRateModifierHdl>(static_cast<const SetSampleRateModifierRequest&>(request), session);
        break;
    case VclType::GetSampleRateModifier:
        return mocca::make_unique<GetSampleRateModifierHdl>(static_cast<const GetSampleRateModifierRequest&>(request), session);
        break;
    case VclType::SetBoundingBoxMode:
        return mocca::make_unique<SetBoundingBoxModeHdl>(static_cast<const SetBoundingBoxModeRequest&>(request), session);
        break;
    case VclType::GetBoundingBoxMode:
        return mocca::make_unique<GetBoundingBoxModeHdl>(static_cast<const GetBoundingBoxModeRequest&>(request), session);
        break;
    case VclType::SetRendererSpecials:
        return mocca::make_unique<SetRendererSpecialsHdl>(static_cast<const SetRendererSpecialsRequest&>(request), session);
        break;
    case VclType::GetRendererSpecials:
        return mocca::make_unique<GetRendererSpecialsHdl>(static_cast<const GetRendererSpecialsRequest&>(request), session);
        break;
    case VclType::SetBackgroundColors:
        return mocca::make_unique<SetBackgroundColorsHdl>(static_cast<const SetBackgroundColorsRequest&>(request), session);
        break;
    case VclType::GetBackgroundColors:
        return mocca::make_unique<GetBackgroundColorsHdl>(static_cast<const GetBackgroundColorsRequest&>(request), session);
        break;
    case VclType::EnableClipping:
        return mocca::make_unique<EnableClippingHdl>(static_cast<const EnableClippingRequest&>(request), session);
        break;
    case VclType::SetClipVolume:
        return mocca::make_unique<SetClipVolumeHdl>(static_cast<const SetClipVolumeRequest&>(request), session);
        break;
    /* AUTOGEN ProcCommandFactoryEntry */
    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}
