#include "VisStream.h"

using namespace trinity::common;



VisStream::VisStream(StreamingParams params) : m_streamingParams(params), m_tail(0), m_head(0) {
}

VisStream::~VisStream() {
    
}


const StreamingParams& VisStream::getStreamingParams() const {
    return m_streamingParams;
}


bool VisStream::put(Frame frame) {
    const auto currentTail = m_tail.load(std::memory_order_relaxed);
    const auto nextTail = increment(currentTail);
    
    // still consistent?
    if (nextTail != m_head.load(std::memory_order_acquire)) {
        m_data[currentTail] = std::move(frame);
        m_tail.store(nextTail, std::memory_order_release);
        return true;
    }
    return false; // full queue
}


Frame VisStream::get() {
    const auto currentHead = m_head.load(std::memory_order_relaxed);
    if (currentHead == m_tail.load(std::memory_order_acquire)) {
        return nullptr;
    }
    
    Frame result = std::move(m_data[currentHead]);
    m_head.store(increment(currentHead), std::memory_order_release);
    return result;
}



size_t VisStream::increment(size_t idx) const {
    return (idx + 1) % CAPACITY;
}
