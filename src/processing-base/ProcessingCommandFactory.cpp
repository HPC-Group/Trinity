#include "ProcessingCommandFactory.h"

#include "commands/ProcessingCommands.h"
#include "commands/ISerialObjectFactory.h"
#include "ProcessingCommandsHandler.h"

#include "mocca/base/Error.h"

using namespace trinity::commands;
using namespace trinity::processing;

std::unique_ptr<ICommandHandler>
ProcessingCommandFactory::createHandler(std::istream& stream) {

    auto serialRequest = ISerialObjectFactory::create();
    serialRequest->readFrom(stream);
    
    VclType type = serialRequest->getType();

    switch (type) {
        case VclType::InitRenderer: {
            InitProcessingSessionCmd cmd(*serialRequest);
            return  std::unique_ptr<InitProcessingSessionHdl> (new InitProcessingSessionHdl(cmd));
            break;
        }
            
        case VclType::SetIsoValue: {
            SetIsoValueCmd cmd(*serialRequest);
            return  std::unique_ptr<SetIsoValueHdl> (new SetIsoValueHdl(cmd));
            break;
        }
            
        default:
            throw mocca::Error("command unknown: " + (Vcl().toString(type)), __FILE__, __LINE__);
    }
}