#pragma once

#include <istream>
#include <map>

#include "commands/ICommandFactory.h"
#include "commands/ICommandHandler.h"

namespace trinity {
namespace processing {

class ProcessingCommandFactory : public commands::ICommandFactory {

public:
    std::unique_ptr<commands::ICommandHandler> createHandler(std::istream&);
};
}
}