#include "ProcessingCommandFactory.h"
#include "common/InitRendererCmd.h"
#include "InitRendererHdl.h"
#include "mocca/base/Error.h"

using namespace trinity::common;
using namespace trinity::processing;

std::unique_ptr<ICommandHandler>
ProcessingCommandFactory::createHandler(std::istream& stream) {
    std::string typeString;
    stream >> typeString;
    
    VclType type = m_vcl.toType(typeString);

    switch (type) {
        case VclType::InitRenderer: {
            InitRendererCmd cmd;
            cmd.deserialize(stream);
            return  std::unique_ptr<InitRendererHdl> (new InitRendererHdl(cmd));
            break;
        }
            
        default:
            throw mocca::Error("command unknown: " + typeString, __FILE__, __LINE__);
    }
}