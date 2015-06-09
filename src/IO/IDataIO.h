//
//  IDataIO.h
//  TNG
//
//  Created by Andre Waschk on 05/02/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef TNG_IDataIO_h
#define TNG_IDataIO_h

#include <Core/Math/Vectors.h>
#include <tuple>

typedef std::tuple<size_t, size_t, size_t> BrickKey; ///< timestep + LOD + 1D brick index

namespace Tuvok{
  class MinMaxBlock;


  /*! \brief basic dummy interface used by the gridleaper and volume pool
  *
  *	Each function declared in this class is used by the volume pool and/or gridleaer
  *	the communication between render node and IO node should implement each function.
  */
  class IDataIO{
  public:
    virtual ~IDataIO(){};

    /// @returns the largest brick which *actually* got used when splitting up
    /// the data set.  This can be smaller than GetMaxBrickSize, when, for
    /// example, the domain is smaller than the brick size requested.
    virtual Core::Math::Vec3ui GetMaxUsedBrickSizes() const = 0;

    /// @returns the min/max scalar and gradient values for the given brick.
    virtual Tuvok::MinMaxBlock MaxMinForKey(const BrickKey&) const=0;

    virtual unsigned GetLODLevelCount() const = 0;
    /// @todo FIXME, should be pure virtual && overridden in derived
    virtual uint64_t GetNumberOfTimesteps() const { return 1; }
    virtual Core::Math::Vec3ui64 GetDomainSize(const size_t lod=0,
                                        const size_t ts=0) const = 0;
    virtual Core::Math::Vec3d GetScale() const = 0;
    virtual Core::Math::Vec3ui GetBrickOverlapSize() const = 0;

    /// @return the LOD idx for a large 1-brick LOD.
    virtual size_t GetLargestSingleBrickLOD(size_t ts) const=0;

    /// Gets the number of voxels, per dimension.
    virtual Core::Math::Vec3ui GetBrickVoxelCounts(const BrickKey&) const = 0;
    /// World space extents.
    virtual Core::Math::Vec3f GetBrickExtents(const BrickKey &) const = 0;
    /// Data access
    ///@{
    virtual bool GetBrick(const BrickKey&, std::vector<uint8_t>&) const=0;  //<- better use template
    virtual bool GetBrick(const BrickKey&, std::vector<int8_t>&) const = 0;
    virtual bool GetBrick(const BrickKey&, std::vector<uint16_t>&) const=0;
    virtual bool GetBrick(const BrickKey&, std::vector<int16_t>&) const=0;
    virtual bool GetBrick(const BrickKey&, std::vector<uint32_t>&) const=0;
    virtual bool GetBrick(const BrickKey&, std::vector<int32_t>&) const=0;
    virtual bool GetBrick(const BrickKey&, std::vector<float>&) const=0;
	virtual bool GetBrick(const BrickKey&, std::vector<double>&) const = 0;

    //this function is used for qvis volumes to return the pointer to the volume
    virtual void* GetBrick() = 0;

    virtual unsigned GetBitWidth() const = 0;
    virtual uint64_t GetComponentCount() const = 0;
    virtual bool GetIsSigned() const = 0;
    virtual bool GetIsFloat() const = 0;


    virtual Core::Math::Vec3ui GetBrickLayout(size_t LoD, size_t timestep) const=0;


    virtual Core::Math::Vec4ui IndexTo4D(const BrickKey& key) const = 0;

	/// @returns the brick key (1D brick index) derived from the 4D key.
	virtual BrickKey IndexFrom4D(const Core::Math::Vec4ui& four,size_t timestep) const = 0;

	virtual std::pair<double, double> GetRange() const = 0;
  };
  typedef std::shared_ptr<IDataIO> DataIOPtr;
}

#endif
