#include "ProcessingCommandFactory.h"
#include "common/InitRendererCmd.h"
#include "common/ISerialObjectFactory.h"
#include "InitRendererHdl.h"
#include "mocca/base/Error.h"

using namespace trinity::common;
using namespace trinity::processing;

std::unique_ptr<ICommandHandler>
ProcessingCommandFactory::createHandler(std::istream& stream) {

    auto serialRequest = ISerialObjectFactory::create();
    serialRequest->readFrom(stream);
    
    VclType type = serialRequest->getType();

    switch (type) {
        case VclType::InitRenderer: {
            InitRendererCmd cmd;
            cmd.deserialize(*serialRequest);
            return  std::unique_ptr<InitRendererHdl> (new InitRendererHdl(cmd));
            break;
        }
            
        default:
            throw mocca::Error("command unknown: " + (Vcl().toString(type)), __FILE__, __LINE__);
    }
}