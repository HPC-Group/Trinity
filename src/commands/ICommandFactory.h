#pragma once

#include <istream>
#include <map>

#include "ICommand.h"
#include "ICommandHandler.h"
#include "Vcl.h"

namespace trinity {
namespace commands {

class ICommandFactory {

public:
    virtual std::unique_ptr<ICommandHandler> createHandler(std::istream&) = 0;
};
}
}