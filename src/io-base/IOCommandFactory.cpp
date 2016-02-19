#include "IOCommandFactory.h"
#include "IOCommandsHandler.h"

#include "commands/ISerialObjectFactory.h"
#include "commands/IOCommands.h"
#include "IOCommandsHandler.h"

#include "mocca/base/Error.h"

using namespace trinity::commands;
using namespace trinity::io;

std::unique_ptr<ICommandHandler>
IOCommandFactory::createHandler(std::istream& stream) {

    auto serialRequest = ISerialObjectFactory::create();
    serialRequest->readFrom(stream);
    
    VclType type = serialRequest->getType();

    switch (type) {
            
        case VclType::InitIOSession: {
            
            InitIOSessionCmd cmd(*serialRequest);
            return  std::unique_ptr<InitIOSessionHdl> (new InitIOSessionHdl(cmd));
            break;
        }
            
        case VclType::GetLODLevelCount: {
            
            GetLODLevelCountCmd cmd(*serialRequest);
            return  std::unique_ptr<GetLODLevelCountHdl> (new GetLODLevelCountHdl(cmd));
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