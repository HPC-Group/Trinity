#pragma once
#include "Commands.h"
#include "ICommand.h"

namespace trinity {
namespace common {

// command-pattern like execution of trinity commands
class ICommandHandler {
    
public:
    virtual void execute() = 0;
    virtual std::unique_ptr<ICommand> getReturnValue() { return nullptr; }
};
}
}