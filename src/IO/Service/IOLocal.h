//
//  IOService.h
//  DemoRendering
//
//  Created by Andre Waschk on 24/10/14.
//  Copyright (c) 2014 CoViDaG. All rights reserved.
//

#ifndef __IOLocal__
#define __IOLocal__

#pragma once

#include <IO/Dataset.h>
#include <IO/LinearIndexDataset.h>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <string>
#include <core/Math/Vectors.h>

#include "DSEntry.h"

struct HandleLocal {
    uint16_t clientID;
    int16_t customHandle;
    std::string host;
};

namespace Tuvok {

class IOLocal
{
public:
    IOLocal(std::string serviceName);
    ~IOLocal();

private:

    std::atomic<uint16_t> _dsHandleSource;
    std::recursive_mutex _dsMutex;
    std::unordered_map<uint16_t, DSEntry> _handleToDS;
    std::unordered_map<std::string, DSEntry> _filenameToDs;

    std::shared_ptr<LinearIndexDataset> getDataset(uint16_t datasetHandle);

    std::shared_ptr<LinearIndexDataset> m_dataset;

public:

    //For definitions of the functions, see Protocol_IO class!

    std::vector<std::string> getFileList();
    uint16_t openFile(std::string filename);
    void closeDataset();

    Vec3ui GetMaxBrickSize();
    Vec3ui GetMaxUsedBrickSizes();
    MinMaxBlock MaxMinForKey(const BrickKey&);

    unsigned GetLODLevelCount();
    uint64_t GetNumberOfTimesteps();
    Vec3ui64 GetDomainSize(const size_t lod=0, const size_t ts=0);
    Vec3d GetScale();
    Vec3ui GetBrickOverlapSize();
    size_t GetLargestSingleBrickLOD(size_t ts);
    Vec3ui GetBrickVoxelCounts(const BrickKey&);
    Vec3f GetBrickExtents(const BrickKey &);
    Vec3ui GetBrickLayout(size_t LoD, size_t timestep);

    unsigned GetBitWidth();
    uint64_t GetComponentCount();
    bool GetIsSigned();
    bool GetIsFloat();
	uint64_t GetTotalBrickCount();

    Core::Math::Vec4ui IndexTo4D(const BrickKey& key);
    BrickKey IndexFrom4D(  const Vec4ui& four, size_t timestep);

	Core::Math::Vec2f GetRange( );

    bool GetBrick(  const BrickKey&, std::vector< uint8_t>&);
    bool GetBrick(  const BrickKey&, std::vector<uint16_t>&);
    bool GetBrick(  const BrickKey&, std::vector<uint32_t>&);
    //bool GetBrick(  const BrickKey&, std::vector<uint64_t>&);
    bool GetBrick(  const BrickKey&, std::vector<  int8_t>&);
    bool GetBrick(  const BrickKey&, std::vector< int16_t>&);
    bool GetBrick(  const BrickKey&, std::vector< int32_t>&);
    //bool GetBrick(  const BrickKey&, std::vector< int64_t>&) override;
    bool GetBrick(  const BrickKey&, std::vector<   float>&);
    bool GetBrick(  const BrickKey&, std::vector<  double>&);
};
}



#endif /* defined(__IOService__) */
