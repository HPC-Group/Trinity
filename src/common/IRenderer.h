#pragma once
#include <memory>
#include <vector>

#include "VisStream.h"


namespace trinity {
namespace common {
    
class IRenderer {
    
public:
    
    IRenderer(std::shared_ptr<VisStream> s) : m_visStream(s) {};
    virtual ~IRenderer() {};
    std::shared_ptr<VisStream> getVisStream() { return m_visStream; };
    
    // base rendering api
    virtual void setIsoValue(const float) = 0;
    
protected:
    std::shared_ptr<VisStream> m_visStream;
};
}
}