#pragma once
#include <memory>


namespace trinity {
namespace common {
    
class IIO {
    
public:
    
    virtual int getLODLevelCount() const = 0;
    
};
}
}