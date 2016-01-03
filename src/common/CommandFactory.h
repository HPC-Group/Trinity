#pragma once

#include <map>
#include <istream>

#include "ICommandHandler.h"
#include "ICommand.h"
#include "Commands.h"

namespace trinity {
namespace common {
    
class CommandFactory {
    
public:
    
    std::unique_ptr<ICommandHandler> createHandler(std::istream&);
    
private:
    Vcl m_vcl;
};
}
}