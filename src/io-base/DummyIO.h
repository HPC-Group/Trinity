#pragma once
#include <iostream>
#include <memory>

#include "common/IIO.h"


namespace trinity {
namespace io {

class DummyIO : public common::IIO {

public:
    // testing purpose
    int getLODLevelCount() const override { return 42; }
};
}
}