#pragma once

#include "commands/ICommandFactory.h"
#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include <memory>

namespace trinity {
namespace processing {

class ProcessingCommandFactory : public commands::ICommandFactory {
public:
    std::unique_ptr<commands::ICommandHandler> createHandler(const commands::Request& request) override;
};
}
}