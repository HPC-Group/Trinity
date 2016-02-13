#pragma once
#include <memory>
#include <iostream>

#include "common/IIO.h"


namespace trinity {
namespace io {
    
    class DummyIO : public common::IIO {
        
    public:
        
        // testing purpose
        int getLODLevelCount()  {
            return 42; }
};
}
}