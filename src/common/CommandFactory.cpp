#include "CommandFactory.h"
#include "InitDummyRendererCmd.h"
#include "InitDummyRendererHdl.h"

using namespace trinity::common;

std::unique_ptr<ICommandHandler> CommandFactory::createHandler(std::istream& stream) {
    std::string typeString;
    int version, sid, rid;
    stream >> version >> sid >> rid >> typeString;
    
    VclType type = m_vcl.toType(typeString);

    switch (type) {
        case VclType::InitDummyRenderer:
            return  std::unique_ptr<InitDummyRendererHdl>
            (new InitDummyRendererHdl(InitDummyRendererCmd(sid, rid)));
            break;
            
        default:
            break;
    }
}