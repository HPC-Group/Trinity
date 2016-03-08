#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

namespace trinity {
class ICommandFactory {
public:
    virtual std::unique_ptr<ICommandHandler> createHandler(const Request& request) = 0;
};
}