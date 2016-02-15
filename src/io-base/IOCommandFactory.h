#pragma once

#include <map>
#include <istream>

#include "commands/ICommandFactory.h"

namespace trinity {
namespace io {
    
class IOCommandFactory : public commands::ICommandFactory {
    
public:
    
    virtual std::unique_ptr<commands::ICommandHandler> createHandler(std::istream&);
    
};
}
}