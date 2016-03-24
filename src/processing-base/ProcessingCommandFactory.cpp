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

#define PYTHON_MAGIC

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
#undef PYTHON_MAGIC
    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}
