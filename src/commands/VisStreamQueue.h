#pragma once
#include <atomic>
#include <cassert>
#include <memory>
#include <thread>

#include "mocca/base/MessageQueue.h"

#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"

namespace trinity {

using Frame = std::vector<uint8_t>;

class VisStreamQueue {
public:
    VisStreamQueue(StreamingParams params);

    const StreamingParams& getStreamingParams() const;

    void put(Frame frame); // false if stream full
    mocca::Nullable<Frame> get();

private:
    StreamingParams m_streamingParams;
    mocca::MessageQueue<Frame> m_queue;
};
}