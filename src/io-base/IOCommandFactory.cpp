#include "io-base/IOCommandFactory.h"

#include "commands/IOCommands.h"
#include "commands/ISerialObjectFactory.h"
#include "io-base/IOCommandsHandler.h"

#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"

using namespace trinity::commands;
using namespace trinity::io;

std::unique_ptr<ICommandHandler> IOCommandFactory::createHandler(const Request& request) {
    VclType type = request.getType();

    switch (type) {
    case VclType::InitIOSession: {
        return mocca::make_unique<InitIOSessionHdl>(static_cast<const InitIOSessionRequest&>(request));
        break;
    }

    case VclType::GetLODLevelCount: {
        return mocca::make_unique<GetLODLevelCountHdl>(static_cast<const GetLODLevelCountRequest&>(request));
        break;
    }

    case VclType::ListFiles: {
        /*
        SetIsoValueCmd cmd(*serialRequest);
        return  std::unique_ptr<SetIsoValueHdl> (new SetIsoValueHdl(cmd));
         */
        throw mocca::Error("command not implemented: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
        break;
    }

    default:
        throw mocca::Error("command unknown: " + (Vcl::instance().toString(type)), __FILE__, __LINE__);
    }
}