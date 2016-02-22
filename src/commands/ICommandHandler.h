#pragma once

#include "ICommand.h"
#include "Vcl.h"

namespace trinity {
namespace commands {

// command-pattern like execution of trinity commands
class ICommandHandler {

public:
    ~ICommandHandler() {}
    virtual void execute() = 0;
    virtual std::unique_ptr<ICommand> getReturnValue() { return nullptr; }
};
}
}