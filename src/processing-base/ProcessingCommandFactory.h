#pragma once

#include <map>
#include <istream>

#include "commands/ICommandHandler.h"
#include "commands/ICommandFactory.h"

namespace trinity {
namespace processing {
    
class ProcessingCommandFactory : public commands::ICommandFactory {
    
public:
    
    std::unique_ptr<commands::ICommandHandler> createHandler(std::istream&);
    
};
}
}