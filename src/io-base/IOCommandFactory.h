#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include <memory>

namespace trinity {

class IONode;
class IOSession;

class IONodeCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request, IONode* node) const;
};

class IOSessionCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request, IOSession* session) const;
};
}