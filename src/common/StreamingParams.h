#pragma once
#include "ISerializable.h"

namespace trinity {
namespace common {
    
class StreamingParams : public ISerializable {

public:
    StreamingParams();
    StreamingParams(int resX, int resY);
    
    virtual VclType getType() const;
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    
    int getResX() const;
    int getResY() const;
    
private:
   int  m_resX, m_resY;
};
}
}