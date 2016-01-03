#pragma once
#include "ISerializable.h"

namespace trinity {
namespace common {
    
class StreamingParams : public ISerializable {

public:
    StreamingParams();
    StreamingParams(int resX, int resY);
    
    void serialize(std::ostream& stream);
    void deserialize(std::istream& stream);
    
    int getResX() const;
    int getResY() const;
    
private:
   int  m_resX, m_resY;
};
}
}