#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include <memory>

namespace trinity {

class IONodeCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) const;
};

class IOSessionCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) const;
};
}