#pragma once

#include <map>
#include <istream>

#include "common/ICommandHandler.h"
#include "common/ICommand.h"
#include "common/Commands.h"

namespace trinity {
namespace processing {
    
class ProcessingCommandFactory {
    
public:
    
    std::unique_ptr<common::ICommandHandler> createHandler(std::istream&);
    
private:
    common::Vcl m_vcl;
};
}
}