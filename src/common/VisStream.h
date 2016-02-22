#pragma once
#include <atomic>
#include <cassert>
#include <memory>
#include <thread>

#include "mocca/base/ByteArray.h"

#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"

namespace trinity {
namespace common {

typedef std::unique_ptr<mocca::ByteArray> Frame;

class VisStream {

#define CAPACITY 3

public:
    // read/write access to the last (newest) frame

    VisStream(commands::StreamingParams params);
    ~VisStream(); // free buffers here

    const commands::StreamingParams& getStreamingParams() const;

    bool put(Frame frame); // false if stream full
    Frame get();


private:
    // based on my lfq implementation
    size_t increment(size_t idx) const;
    std::atomic<size_t> m_tail;
    std::atomic<size_t> m_head;
    Frame m_data[CAPACITY];

    commands::StreamingParams m_streamingParams;
};
}
}