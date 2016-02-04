#pragma once
#include "ISerializable.h"

namespace trinity {
namespace common {
    
class StreamingParams : public ISerializable {

public:
    StreamingParams();
    StreamingParams(int resX, int resY);
    
    virtual void serialize(ISerialObject& serial);
    virtual void deserialize(ISerialObject& serial);
    
    int getResX() const;
    int getResY() const;
    
private:
   int  m_resX, m_resY;
};
}
}