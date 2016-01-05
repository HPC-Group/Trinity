#pragma once
#include <atomic>
#include <memory>
#include <thread>
#include <cassert>
#include "mocca/base/ByteArray.h"
#include "Commands.h"
#include "StreamingParams.h"

namespace trinity {
namespace common {
    
class VisStream {

#define CAPACITY 2
    
public:
    
    // read/write access to the last (newest) frame
    
    VisStream(StreamingParams params);
    ~VisStream();  // free buffers here
    
    const StreamingParams& getStreamingParams() const;
    
    bool put(Frame frame); // false if stream full
    Frame get();
    
    
private:

    // based on my lfq implementation
    size_t increment(size_t idx) const;
    std::atomic<size_t> m_tail;
    std::atomic<size_t> m_head;
    Frame m_data[CAPACITY];
    
    StreamingParams m_streamingParams;
};
}
}