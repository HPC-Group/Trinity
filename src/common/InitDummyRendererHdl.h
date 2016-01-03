#pragma once
#include "ICommandHandler.h"
#include "InitDummyRendererCmd.h"

namespace trinity {
namespace common {

// command-pattern like execution of trinity commands
class InitDummyRendererHdl : public ICommandHandler {
    
public:
    virtual void execute();
    InitDummyRendererHdl(InitDummyRendererCmd cmd);
    ~InitDummyRendererHdl();
};
}
}