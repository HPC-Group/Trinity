#include "io-base/IOCommandFactory.h"

#include "common/TrinityError.h"
#include "commands/IOCommands.h"
#include "io-base/IOCommandsHandler.h"

#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"

using namespace trinity::commands;
using namespace trinity::common;
using namespace trinity::io;

std::unique_ptr<ICommandHandler> IOCommandFactory::createHandler(const Request& request) {
    VclType type = request.getType();

    switch (type) {
    case VclType::InitIOSession:
        return mocca::make_unique<InitIOSessionHdl>(static_cast<const InitIOSessionRequest&>(request));
        break;
    case VclType::GetLODLevelCount:
        return mocca::make_unique<GetLODLevelCountHdl>(static_cast<const GetLODLevelCountRequest&>(request));
        break;
    case VclType::ListFiles:
        return mocca::make_unique<ListFilesHdl>(static_cast<const ListFilesRequest&>(request));
        break;
    default:
        throw TrinityError("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}