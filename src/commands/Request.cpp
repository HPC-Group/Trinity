#include "commands/Request.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/SerializerFactory.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity;

std::unique_ptr<Request> Request::createRequestInternal(const ISerialReader& reader) {
    VclType type = Vcl::instance().toType(reader.getString("type"));
    // processing commands
    if (type == InitProcessingSessionRequest::Ifc::Type) {
        return reader.getSerializablePtr<InitProcessingSessionRequest>("req");
    } else if (type == SetIsoValueRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetIsoValueRequest>("req");
    } else if (type == InitContextRequest::Ifc::Type) {
        return reader.getSerializablePtr<InitContextRequest>("req");
    } else if (type == ZoomCameraRequest::Ifc::Type) {
        return reader.getSerializablePtr<ZoomCameraRequest>("req");
    } else if (type == SetRenderModeRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetRenderModeRequest>("req");
    } else if (type == SupportsRenderModeRequest::Ifc::Type) {
        return reader.getSerializablePtr<SupportsRenderModeRequest>("req");
    } else if (type == SetActiveModalityRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetActiveModalityRequest>("req");
    } else if (type == GetActiveModalityRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetActiveModalityRequest>("req");
    } else if (type == SetActiveTimestepRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetActiveTimestepRequest>("req");
    } else if (type == GetActiveTimestepRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetActiveTimestepRequest>("req");
    } else if (type == GetModalityCountProcRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetModalityCountProcRequest>("req");
    } else if (type == GetTimestepCountProcRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetTimestepCountProcRequest>("req");
    } else if (type == Set1DTransferFunctionRequest::Ifc::Type) {
        return reader.getSerializablePtr<Set1DTransferFunctionRequest>("req");
    } else if (type == GetDefault1DTransferFunctionProcRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetDefault1DTransferFunctionProcRequest>("req");
    } else if (type == GetDefault1DTransferFunctionCountProcRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetDefault1DTransferFunctionCountProcRequest>("req");
    } else if (type == Get1DHistogramProcRequest::Ifc::Type) {
        return reader.getSerializablePtr<Get1DHistogramProcRequest>("req");
    } else if (type == GetIsoValueRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetIsoValueRequest>("req");
    } else if (type == SetIsosurfaceColorRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetIsosurfaceColorRequest>("req");
    } else if (type == GetIsosurfaceColorRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetIsosurfaceColorRequest>("req");
    } else if (type == GetRangeProcRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetRangeProcRequest>("req");
    } else if (type == SetClearViewPositionRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetClearViewPositionRequest>("req");
    } else if (type == SetClearViewRadiusRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetClearViewRadiusRequest>("req");
    } else if (type == SetClearBorderSizeRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetClearBorderSizeRequest>("req");
    } else if (type == EnableLightingRequest::Ifc::Type) {
        return reader.getSerializablePtr<EnableLightingRequest>("req");
    } else if (type == SetLightingColorsRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetLightingColorsRequest>("req");
    } else if (type == GetLightingColorsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetLightingColorsRequest>("req");
    } else if (type == SetLightDirectionRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetLightDirectionRequest>("req");
    } else if (type == GetLightDirectionRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetLightDirectionRequest>("req");
    } else if (type == SetSampleRateModifierRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetSampleRateModifierRequest>("req");
    } else if (type == GetSampleRateModifierRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetSampleRateModifierRequest>("req");
    } else if (type == SetBoundingBoxModeRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetBoundingBoxModeRequest>("req");
    } else if (type == GetBoundingBoxModeRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBoundingBoxModeRequest>("req");
    } else if (type == SetRendererSpecialsRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetRendererSpecialsRequest>("req");
    } else if (type == GetRendererSpecialsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetRendererSpecialsRequest>("req");
    } else if (type == SetBackgroundColorsRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetBackgroundColorsRequest>("req");
    } else if (type == GetBackgroundColorsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBackgroundColorsRequest>("req");
    } else if (type == EnableClippingRequest::Ifc::Type) {
        return reader.getSerializablePtr<EnableClippingRequest>("req");
    } else if (type == SetClipVolumeRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetClipVolumeRequest>("req");
    } else if (type == SetViewParametersRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetViewParametersRequest>("req");
    } else if (type == RotateCameraRequest::Ifc::Type) {
        return reader.getSerializablePtr<RotateCameraRequest>("req");
    } else if (type == MoveCameraRequest::Ifc::Type) {
        return reader.getSerializablePtr<MoveCameraRequest>("req");
    } else if (type == RotateSceneRequest::Ifc::Type) {
        return reader.getSerializablePtr<RotateSceneRequest>("req");
    } else if (type == MoveSceneRequest::Ifc::Type) {
        return reader.getSerializablePtr<MoveSceneRequest>("req");
    } else if (type == RescaleSceneRequest::Ifc::Type) {
        return reader.getSerializablePtr<RescaleSceneRequest>("req");
    } else if (type == ResetCameraRequest::Ifc::Type) {
        return reader.getSerializablePtr<ResetCameraRequest>("req");
    } else if (type == ResetSceneRequest::Ifc::Type) {
        return reader.getSerializablePtr<ResetSceneRequest>("req");
    } else if (type == StartRenderingRequest::Ifc::Type) {
        return reader.getSerializablePtr<StartRenderingRequest>("req");
    } else if (type == StopRenderingRequest::Ifc::Type) {
        return reader.getSerializablePtr<StopRenderingRequest>("req");
    } else if (type == IsIdleRequest::Ifc::Type) {
        return reader.getSerializablePtr<IsIdleRequest>("req");
    } else if (type == ProceedRenderingRequest::Ifc::Type) {
        return reader.getSerializablePtr<ProceedRenderingRequest>("req");
    } else if (type == SetUserViewMatrixRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetUserViewMatrixRequest>("req");
    } else if (type == SetUserWorldMatrixRequest::Ifc::Type) {
        return reader.getSerializablePtr<SetUserWorldMatrixRequest>("req");
    }
    /* AUTOGEN ProcRequestFactoryEntry */

    // IO commands
    else if (type == ListFilesRequest::Ifc::Type) {
        return reader.getSerializablePtr<ListFilesRequest>("req");
    } else if (type == InitIOSessionRequest::Ifc::Type) {
        return reader.getSerializablePtr<InitIOSessionRequest>("req");
    } else if (type == GetLODLevelCountRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetLODLevelCountRequest>("req");
    } else if (type == GetMaxBrickSizeRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetMaxBrickSizeRequest>("req");
    } else if (type == GetMaxUsedBrickSizesRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetMaxUsedBrickSizesRequest>("req");
    } else if (type == GetMaxMinForKeyRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetMaxMinForKeyRequest>("req");
    } else if (type == GetNumberOfTimestepsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetNumberOfTimestepsRequest>("req");
    } else if (type == GetDomainSizeRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetDomainSizeRequest>("req");
    } else if (type == GetTransformationRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetTransformationRequest>("req");
    } else if (type == GetBrickOverlapSizeRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBrickOverlapSizeRequest>("req");
    } else if (type == GetLargestSingleBrickLODRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetLargestSingleBrickLODRequest>("req");
    } else if (type == GetBrickVoxelCountsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBrickVoxelCountsRequest>("req");
    } else if (type == GetBrickExtentsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBrickExtentsRequest>("req");
    } else if (type == GetBrickLayoutRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBrickLayoutRequest>("req");
    } else if (type == GetModalityCountRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetModalityCountRequest>("req");
    } else if (type == GetComponentCountRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetComponentCountRequest>("req");
    } else if (type == GetRangeRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetRangeRequest>("req");
    } else if (type == GetTotalBrickCountRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetTotalBrickCountRequest>("req");
    } else if (type == GetBrickRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBrickRequest>("req");
    } else if (type == GetTypeRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetTypeRequest>("req");
    } else if (type == GetSemanticRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetSemanticRequest>("req");
    } else if (type == GetDefault1DTransferFunctionCountRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetDefault1DTransferFunctionCountRequest>("req");
    } else if (type == Get1DHistogramRequest::Ifc::Type) {
        return reader.getSerializablePtr<Get1DHistogramRequest>("req");
    } else if (type == Get2DHistogramRequest::Ifc::Type) {
        return reader.getSerializablePtr<Get2DHistogramRequest>("req");
    } else if (type == GetUserDefinedSemanticRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetUserDefinedSemanticRequest>("req");
    } else if (type == GetDefault1DTransferFunctionRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetDefault1DTransferFunctionRequest>("req");
    } else if (type == GetDomainScaleRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetDomainScaleRequest>("req");
    } else if (type == GetFloatBrickLayoutRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetFloatBrickLayoutRequest>("req");
    } else if (type == GetBrickMaxMinRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetBrickMaxMinRequest>("req");
    } else if (type == GetRootsRequest::Ifc::Type) {
        return reader.getSerializablePtr<GetRootsRequest>("req");
    }
    /* AUTOGEN IORequestFactoryEntry */

    throw TrinityError("Invalid request type", __FILE__, __LINE__);
}

std::unique_ptr<Request> Request::createFromMessage(const mocca::net::Message& message) {
    auto reader = ISerializerFactory::defaultFactory().createReader(message);
    auto request = createRequestInternal(*reader);
    return request;
}

mocca::net::Message Request::createMessage(const Request& request) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->appendString("type", Vcl::instance().toString(request.getType()));
    writer->appendObject("req", request);
    return writer->writeMessage();
}

namespace trinity {
std::ostream& operator<<(std::ostream& os, const Request& obj) {
    return os << obj.toString();
}
}
