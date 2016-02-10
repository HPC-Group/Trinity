#include "IOCommandFactory.h"
#include "common/ISerialObjectFactory.h"

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
            /*
            InitRendererCmd cmd(*serialRequest);
            return  std::unique_ptr<InitRendererHdl> (new InitRendererHdl(cmd));
             */
            break;
        }
        
        case VclType::ListFiles: {
            /*
            SetIsoValueCmd cmd(*serialRequest);
            return  std::unique_ptr<SetIsoValueHdl> (new SetIsoValueHdl(cmd));
             */
            break;
        }
            
        default:
            throw mocca::Error("command unknown: " + (Vcl().toString(type)), __FILE__, __LINE__);
    }
}