#pragma once

#include "commands/ProcessingCommands.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/Nullable.h"

#include <condition_variable>
#include <mutex>

namespace trinity {

using Frame = mocca::ByteArray;

class VisStream {
public:
    VisStream(StreamingParams params);

    const StreamingParams& getStreamingParams() const;

    void put(Frame frame);
    mocca::Nullable<Frame> get();

private:
    StreamingParams m_streamingParams;
    Frame m_frame;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};
}