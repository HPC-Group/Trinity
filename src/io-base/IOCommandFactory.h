#pragma once

#include "commands/ICommandFactory.h"
#include "commands/ICommandHandler.h"

namespace trinity {

class IONodeCommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request);
};

class IOCommandFactory : public ICommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;
};
}