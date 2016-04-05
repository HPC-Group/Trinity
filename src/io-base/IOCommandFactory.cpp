#include "io-base/IOCommandFactory.h"

#include "commands/IOCommands.h"
#include "common/TrinityError.h"
#include "io-base/IOCommandsHandler.h"
#include "io-base/IONode.h"
#include "io-base/IOSession.h"

#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"

using namespace trinity;

std::unique_ptr<ICommandHandler> IONodeCommandFactory::createHandler(const Request& request, IONode* node) const {
    VclType type = request.getType();

    switch (type) {
    case VclType::InitIOSession:
        return mocca::make_unique<InitIOSessionHdl>(static_cast<const InitIOSessionRequest&>(request), node);
        break;
    case VclType::ListFiles:
        return mocca::make_unique<ListFilesHdl>(static_cast<const ListFilesRequest&>(request), node);
        break;
    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}

std::unique_ptr<ICommandHandler> IOSessionCommandFactory::createHandler(const Request& request, IOSession* session) const {
    VclType type = request.getType();

    switch (type) {
    case VclType::GetLODLevelCount:
        return mocca::make_unique<GetLODLevelCountHdl>(static_cast<const GetLODLevelCountRequest&>(request), session);
        break;
    case VclType::GetMaxBrickSize:
        return mocca::make_unique<GetMaxBrickSizeHdl>(static_cast<const GetMaxBrickSizeRequest&>(request), session);
        break;
    case VclType::GetMaxUsedBrickSizes:
        return mocca::make_unique<GetMaxUsedBrickSizesHdl>(static_cast<const GetMaxUsedBrickSizesRequest&>(request), session);
        break;
    case VclType::MaxMinForKey:
        return mocca::make_unique<MaxMinForKeyHdl>(static_cast<const MaxMinForKeyRequest&>(request), session);
        break;
    case VclType::GetNumberOfTimesteps:
        return mocca::make_unique<GetNumberOfTimestepsHdl>(static_cast<const GetNumberOfTimestepsRequest&>(request), session);
        break;
    case VclType::GetDomainSize:
        return mocca::make_unique<GetDomainSizeHdl>(static_cast<const GetDomainSizeRequest&>(request), session);
        break;
    case VclType::GetTransformation:
        return mocca::make_unique<GetTransformationHdl>(static_cast<const GetTransformationRequest&>(request), session);
        break;
    case VclType::GetBrickOverlapSize:
        return mocca::make_unique<GetBrickOverlapSizeHdl>(static_cast<const GetBrickOverlapSizeRequest&>(request), session);
        break;
    case VclType::GetLargestSingleBrickLOD:
        return mocca::make_unique<GetLargestSingleBrickLODHdl>(static_cast<const GetLargestSingleBrickLODRequest&>(request), session);
        break;
    case VclType::GetBrickVoxelCounts:
        return mocca::make_unique<GetBrickVoxelCountsHdl>(static_cast<const GetBrickVoxelCountsRequest&>(request), session);
        break;
    case VclType::GetBrickExtents:
        return mocca::make_unique<GetBrickExtentsHdl>(static_cast<const GetBrickExtentsRequest&>(request), session);
        break;
    case VclType::GetBrickLayout:
        return mocca::make_unique<GetBrickLayoutHdl>(static_cast<const GetBrickLayoutRequest&>(request), session);
        break;
    case VclType::GetModalityCount:
        return mocca::make_unique<GetModalityCountHdl>(static_cast<const GetModalityCountRequest&>(request), session);
        break;
    case VclType::GetComponentCount:
        return mocca::make_unique<GetComponentCountHdl>(static_cast<const GetComponentCountRequest&>(request), session);
        break;
    case VclType::GetRange:
        return mocca::make_unique<GetRangeHdl>(static_cast<const GetRangeRequest&>(request), session);
        break;
    case VclType::GetTotalBrickCount:
        return mocca::make_unique<GetTotalBrickCountHdl>(static_cast<const GetTotalBrickCountRequest&>(request), session);
        break;
    case VclType::GetBrick:
        return mocca::make_unique<GetBrickHdl>(static_cast<const GetBrickRequest&>(request), session);
        break;
    case VclType::GetType:
        return mocca::make_unique<GetTypeHdl>(static_cast<const GetTypeRequest&>(request), session);
        break;
    case VclType::GetSemantic:
        return mocca::make_unique<GetSemanticHdl>(static_cast<const GetSemanticRequest&>(request), session);
        break;
    case VclType::GetDefault1DTransferFunctionCount:
        return mocca::make_unique<GetDefault1DTransferFunctionCountHdl>(
            static_cast<const GetDefault1DTransferFunctionCountRequest&>(request), session);
        break;
    case VclType::Get1DHistogram:
        return mocca::make_unique<Get1DHistogramHdl>(static_cast<const Get1DHistogramRequest&>(request), session);
        break;
    case VclType::Get2DHistogram:
        return mocca::make_unique<Get2DHistogramHdl>(static_cast<const Get2DHistogramRequest&>(request), session);
        break;
    case VclType::GetUserDefinedSemantic:
        return mocca::make_unique<GetUserDefinedSemanticHdl>(static_cast<const GetUserDefinedSemanticRequest&>(request), session);
        break;
    case VclType::GetDefault1DTransferFunction:
        return mocca::make_unique<GetDefault1DTransferFunctionHdl>(static_cast<const GetDefault1DTransferFunctionRequest&>(request),
                                                                   session);
        break;
    case VclType::GetDomainScale:
        return mocca::make_unique<GetDomainScaleHdl>(static_cast<const GetDomainScaleRequest&>(request), session);
        break;
    case VclType::GetFloatBrickLayout:
        return mocca::make_unique<GetFloatBrickLayoutHdl>(static_cast<const GetFloatBrickLayoutRequest&>(request), session);
        break;
    case VclType::GetBrickMaxMin:
        return mocca::make_unique<GetBrickMaxMinHdl>(static_cast<const GetBrickMaxMinRequest&>(request), session);
        break;
    case VclType::GetBricks:
        return mocca::make_unique<GetBricksHdl>(static_cast<const GetBricksRequest&>(request), session);
        break;
    /* AUTOGEN IOCommandFactoryEntry */
    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}