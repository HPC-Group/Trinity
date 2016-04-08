#pragma once

#include "commands/ProcessingCommands.h"

#include "mocca/base/Nullable.h"

#include <condition_variable>
#include <mutex>

namespace trinity {

using Frame = std::vector<uint8_t>;

class VisStream {
public:
    VisStream(StreamingParams params);

    const StreamingParams& getStreamingParams() const;

    void put(std::unique_ptr<Frame> frame);
    std::unique_ptr<Frame> get();

private:
    StreamingParams m_streamingParams;
    std::unique_ptr<Frame> m_frame;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};
}