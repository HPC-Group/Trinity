#pragma once

#include "commands/CommandInputChannel.h"
#include "commands/CommandInputChannel.h"
#include "commands/ErrorCommands.h"
#include "commands/Vcl.h"

#include "mocca/log/LogManager.h"

#include <chrono>
#include <thread>

namespace trinity {
namespace common {

template <typename RequestType>
std::unique_ptr<typename RequestType::ReplyType> sendRequestChecked(const commands::CommandInputChannel& channel, const RequestType& request) {
    using ReplyType = typename RequestType::ReplyType;
    channel.sendRequest(request);
    auto reply = channel.getReply();
    if (reply->getType() == commands::VclType::TrinityError) {
        const auto& error = static_cast<const commands::ErrorReply&>(*reply);
        throw mocca::Error("Error received: " + error.getParams().printError(), __FILE__, __LINE__);
    }
    if (reply->getType() != request.getType()) {
        throw mocca::Error("Reply type does not match request type", __FILE__, __LINE__);
    }
    return std::unique_ptr<ReplyType>(static_cast<ReplyType*>(reply.release()));
}

bool connectInputChannel(const commands::CommandInputChannel& inputChannel);
}
}