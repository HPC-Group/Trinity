//
//  IOService.cpp
//  DemoRendering
//
//  Created by Andre Waschk on 24/10/14.
//  Copyright (c) 2014 CoViDaG. All rights reserved.
//

#include "DSEntry.h"

#include <IO/IOManager.h>
#include <IO/TuvokIOError.h>
using namespace std;

namespace Tuvok {
class DSEntryFreed : public std::runtime_error {
public:
    DSEntryFreed() : std::runtime_error("Dataset reached a retain count of 0 and is being freed!") {}
};


DSEntry::DSEntry(string filename, uint16_t handle, shared_ptr<Dataset> ds)
    : _filename(filename), _handle(handle), _ds(ds), _referenceCount(1)
{}
void DSEntry::incrRetainCount() {
    _referenceCount++;
}
void DSEntry::decrRetainCount() {
    _referenceCount--;
    if(_referenceCount <= 0) {
        _ds = nullptr;
        throw DSEntryFreed();
    }
}

string DSEntry::getFilename() const {
    return _filename;
}
uint16_t DSEntry::getHandle() const {
    return _handle;
}
shared_ptr<Dataset> DSEntry::getDataset() const {
    return _ds;
}

}
