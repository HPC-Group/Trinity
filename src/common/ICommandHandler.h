#pragma once
#include "Commands.h"
#include "ICommand.h"
#include "mocca/base/Nullable.h"

namespace trinity {
namespace common {

// command-pattern like execution of trinity commands
class ICommandHandler {
    
public:
    virtual void execute() = 0;
    
    // todo: retvalue
    
    
protected:
    
};
}
}