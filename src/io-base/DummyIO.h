#pragma once
#include <iostream>
#include <memory>

#include "common/IIO.h"


namespace trinity {
class DummyIO : public IIO {
public:
    // testing purpose
    int getLODLevelCount() const override { return 42; }
};
}