#pragma once

#include "commands/CommandInputChannel.h"
#include "commands/ErrorCommands.h"

namespace trinity {
namespace common {

template <typename RequestType>
std::unique_ptr<typename RequestType::ReplyType> sendRequestChecked(commands::CommandInputChannel& channel, const RequestType& request) {
    using ReplyType = typename RequestType::ReplyType;
    channel.sendRequest(request);
    auto reply = channel.getReply();
    if (reply->getType() == VclType::TrinityError) {
        const auto& error = static_cast<const ErrorReply&>(*reply);
        throw mocca::Error("Error received: " + error.getParams().printError(), __FILE__, __LINE__);
    }
    if (reply->getType() != request.getType()) {
        throw mocca::Error("Reply type does not match request type", __FILE__, __LINE__);
    }
    return std::unique_ptr<ReplyType>(static_cast<ReplyType*>(reply.release()));
}
}
}