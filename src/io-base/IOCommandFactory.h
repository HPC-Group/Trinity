#pragma once

#include "commands/ICommandFactory.h"
#include "commands/ICommandHandler.h"

namespace trinity {
namespace io {

class IOCommandFactory : public commands::ICommandFactory {
public:
    std::unique_ptr<commands::ICommandHandler> createHandler(const commands::Request& request) override;
};
}
}