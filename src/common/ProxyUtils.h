#pragma once

#include "commands/CommandInputChannel.h"
#include "commands/CommandInputChannel.h"
#include "commands/ErrorCommands.h"
#include "commands/Vcl.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"

#include <chrono>
#include <thread>

namespace trinity {

template <typename RequestType>
std::unique_ptr<typename RequestType::ReplyType> sendRequestChecked(const CommandInputChannel& channel, const RequestType& request) {
    using ReplyType = typename RequestType::ReplyType;
    channel.sendRequest(request);
    auto reply = channel.getReply();
    if (reply->getType() == VclType::TrinityError) {
        const auto& error = static_cast<const ErrorReply&>(*reply);
        throw TrinityError("Error received: " + error.getParams().getError(), __FILE__, __LINE__);
    }
    if (reply->getType() != request.getType()) {
        throw TrinityError("Reply type does not match request type", __FILE__, __LINE__);
    }
    return std::unique_ptr<ReplyType>(static_cast<ReplyType*>(reply.release()));
}

bool connectInputChannel(const CommandInputChannel& inputChannel);
}