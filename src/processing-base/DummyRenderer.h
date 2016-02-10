#pragma once
#include <memory>

#include "common/IRenderer.h"


namespace trinity {
namespace processing {
    
    
class DummyRenderer : public common::IRenderer {
    
public:
    
    DummyRenderer(std::shared_ptr<common::VisStream> stream);
    virtual void setIsoValue(const float);
    
    
    
    // unit test purposes
    float getIsoValue() const {return m_isoValue; };
private:
    float m_isoValue;
};
}
}