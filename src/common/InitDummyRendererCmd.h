#pragma once
#include "ICommand.h"

namespace trinity {
namespace common {

class InitDummyRendererCmd : public ICommand {
    
public:
    InitDummyRendererCmd(int sid, int rid);
    virtual ~InitDummyRendererCmd();
    
    virtual VclType getType() const;
    
    virtual void serialize(std::ostream& stream);
    virtual void deserialize(std::istream& stream);
};
}
}