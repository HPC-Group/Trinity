#include "IOCommandFactory.h"
#include "IONodeCmdsHdl.h"

#include "common/ISerialObjectFactory.h"
#include "common/IONodeCmds.h"

#include "mocca/base/Error.h"

using namespace trinity::common;
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
             
            throw mocca::Error("command not implemented: " + (Vcl().toString(type)), __FILE__, __LINE__);
            break;
        }
        
        case VclType::ListFiles: {
            /*
            SetIsoValueCmd cmd(*serialRequest);
            return  std::unique_ptr<SetIsoValueHdl> (new SetIsoValueHdl(cmd));
             */
            throw mocca::Error("command not implemented: " + (Vcl().toString(type)), __FILE__, __LINE__);
            break;
        }
            
        default:
            throw mocca::Error("command unknown: " + (Vcl().toString(type)), __FILE__, __LINE__);
    }
}