#pragma once

#include "Vcl.h"
#include "ICommand.h"

namespace trinity {
namespace commands {

// command-pattern like execution of trinity commands
class ICommandHandler {
    
public:
    // dmc: should have a virtual destructor
    // ~ICommandHandler() {}
    virtual void execute() = 0;
    virtual std::unique_ptr<ICommand> getReturnValue() { return nullptr; }
};
}
}