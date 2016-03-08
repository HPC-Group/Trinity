#include "processing-base/ProcessingCommandFactory.h"

#include "processing-base/ProcessingCommandsHandler.h"
#include "commands/ProcessingCommands.h"
#include "common/TrinityError.h"

#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"

#include <iostream>

using namespace trinity;

std::unique_ptr<ICommandHandler> ProcessingCommandFactory::createHandler(const Request& request) {
    VclType type = request.getType();

    switch (type) {
    case VclType::InitRenderer:
        return mocca::make_unique<InitProcessingSessionHdl>(static_cast<const InitProcessingSessionRequest&>(request));
        break;
    case VclType::SetIsoValue:
        return mocca::make_unique<SetIsoValueHdl>(static_cast<const SetIsoValueRequest&>(request));
        break;
    case VclType::InitContext:
        return mocca::make_unique<InitContextHdl>(static_cast<const InitContextRequest&>(request));
        break;
    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}
