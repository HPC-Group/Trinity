#pragma once
#include "ICommand.h"
#include "StreamingParams.h"

namespace trinity {
namespace common {

class InitDummyRendererCmd : public ICommand {
    
public:
    InitDummyRendererCmd(int sid, int rid);
    InitDummyRendererCmd(int sid, int rid, const std::string& protocol, StreamingParams p);
    virtual ~InitDummyRendererCmd();
    
    virtual VclType getType() const;
    
    virtual void serialize(std::ostream& stream);
    virtual void deserialize(std::istream& stream);
    
private:
    std::string m_protocol;
    StreamingParams m_streamingParams;
};
}
}