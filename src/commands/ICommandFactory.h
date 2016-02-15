#pragma once

#include <map>
#include <istream>

#include "ICommandHandler.h"
#include "ICommand.h"
#include "Vcl.h"

namespace trinity {
namespace commands {
    
class ICommandFactory {
    
public:
    
    virtual std::unique_ptr<ICommandHandler> createHandler(std::istream&) = 0;
    
protected:
    Vcl m_vcl;
};
}
}