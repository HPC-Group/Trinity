//
//  IOService.h
//  DemoRendering
//
//  Created by Andre Waschk on 24/10/14.
//  Copyright (c) 2014 CoViDaG. All rights reserved.
//

#ifndef __DSEntry__
#define __DSEntry__

#pragma once

#include <IO/Dataset.h>
#include <IO/LinearIndexDataset.h>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <string>
#include <memory>

namespace Tuvok {

class DSEntry {
public:
    DSEntry(std::string filename, uint16_t handle, std::shared_ptr<Dataset> ds);
    void incrRetainCount();
    void decrRetainCount();

    std::string getFilename() const;
    uint16_t getHandle() const;
    std::shared_ptr<Dataset> getDataset() const;

private:
    std::string              _filename;
    uint16_t            _handle;
    std::shared_ptr<Dataset> _ds;

    uint8_t             _referenceCount;
};

}



#endif /* defined(__IOService__) */
