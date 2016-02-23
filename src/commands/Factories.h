#pragma once

#include "commands/ISerializable.h"
#include "commands/Request.h"
#include "commands/Reply.h"

#include <memory>

namespace trinity {
namespace commands {
std::unique_ptr<Request> createRequest(const ISerialObject& obj);
std::unique_ptr<Reply> createReply(const ISerialObject& obj);
}
}