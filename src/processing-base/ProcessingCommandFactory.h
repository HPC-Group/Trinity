#pragma once

#include "commands/ICommandFactory.h"
#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include <memory>

namespace trinity {
class ProcessingCommandFactory : public ICommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;
};
}