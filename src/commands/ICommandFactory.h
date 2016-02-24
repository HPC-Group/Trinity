#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

namespace trinity {
namespace commands {

class ICommandFactory {

public:
    virtual std::unique_ptr<commands::ICommandHandler> createHandler(const commands::Request& request) = 0;
};
}
}