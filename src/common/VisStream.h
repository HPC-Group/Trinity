#pragma once
#include <atomic>
#include <memory>
#include <thread>
#include <cassert>
#include "mocca/base/ByteArray.h"
#include "Commands.h"

namespace trinity {
namespace common {
    
    
class VisStream {
    
public:
    
    // read/write access to the last (newest) frame
    
    VisStream(StreamParams params);
    ~VisStream();  // free buffers here

    // call after you are done with the last frame
    void swapBuffers();
    
    // you should not read or write buffers while swapping
    bool isSwapping() const;
    
    // call stopFrameRead when done processing the array
    Frame& readLastFrame();
    
    void insertFrame(Frame&);
    
    const StreamParams& getStreamParams() const;
    
    
private:

    std::atomic<bool> m_isSwapping;
    const StreamParams m_streamParams;
    
    // will be extended by an array later
    Frame m_lastFrame;
    Frame m_swapBuffer;
};
}
}