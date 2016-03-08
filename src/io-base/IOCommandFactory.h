#pragma once

#include "commands/ICommandFactory.h"
#include "commands/ICommandHandler.h"

namespace trinity {
class IOCommandFactory : public ICommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;
};
}