#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include <memory>

namespace trinity {
class ProcessingNode;
class RenderSession;

class ProcessingNodeCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request, ProcessingNode* node) const;
};

class ProcessingSessionCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request, RenderSession* session) const;
};
}