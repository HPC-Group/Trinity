#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include <memory>

namespace trinity {
class ProcessingNodeCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) const;
};

class ProcessingSessionCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) const;
};
}