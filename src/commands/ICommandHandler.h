#pragma once

#include "commands/Reply.h"

namespace trinity {
// command-pattern like execution of trinity commands
class ICommandHandler {
public:
    virtual ~ICommandHandler() {}
    virtual std::unique_ptr<Reply> execute() = 0;
};
}