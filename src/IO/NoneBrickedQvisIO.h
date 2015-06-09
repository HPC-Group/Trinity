//
//  NonBrickedQvisIO.h
//  TNG
//
//  Created by Andre Waschk on 24/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__NoneBrickedQvisIO__
#define __TNG__NoneBrickedQvisIO__

#include "IDataIO.h"
#include <core/MinMaxBlock.h>
#include <memory>

namespace DataIO{
  namespace VolumeIO{
    class QVisVolume;
    typedef std::shared_ptr<QVisVolume> QVisVolumePtr;
  }
}

namespace Tuvok{

  /*! \brief class to test the IDataIO for simple, none bricked QVis Volumes

      Consider that most of the functions are not needed by the DemoRaycaster.
      The actual Volume consists of ONE Brick (the raw data)

   */
  class NoneBrickedQvisIO : public IDataIO{
  public:
    NoneBrickedQvisIO();
    virtual ~NoneBrickedQvisIO(){};

    bool load(const std::string& datFile);
    void toString();

    //! return (1,1,1) this volumes are not bricked at all
    virtual Core::Math::Vec3ui GetMaxUsedBrickSizes() const;

    //! there is only one lod
    virtual unsigned GetLODLevelCount() const;

    //! returns the size of the lod, parameters do not metter
    virtual Core::Math::Vec3ui64 GetDomainSize(const size_t lod=0,
                                             const size_t ts=0) const;
    //! for first we don't scale the volume
    virtual Core::Math::Vec3d GetScale() const;

    //! just 1
    virtual size_t GetLargestSingleBrickLOD(size_t ts) const;

    //! just the size of the volume, so == GetDomainSize
    virtual Core::Math::Vec3ui GetBrickVoxelCounts(const BrickKey&) const;

    //stupid... should use templates or so
    virtual bool GetBrick(const BrickKey&, std::vector<uint8_t>&) const;
    virtual bool GetBrick(const BrickKey&, std::vector<int8_t>&) const;
    virtual bool GetBrick(const BrickKey&, std::vector<uint16_t>&) const;
    virtual bool GetBrick(const BrickKey&, std::vector<int16_t>&) const;
    virtual bool GetBrick(const BrickKey&, std::vector<uint32_t>&) const;
    virtual bool GetBrick(const BrickKey&, std::vector<int32_t>&) const;
    virtual bool GetBrick(const BrickKey&, std::vector<float>&) const;
	virtual bool GetBrick(const BrickKey&, std::vector<double>&) const;

    virtual void* GetBrick();

    virtual unsigned GetBitWidth() const;
    virtual uint64_t GetComponentCount() const;
    virtual bool GetIsSigned() const;
    virtual bool GetIsFloat() const;


    //functions below are not used for qvis volume
	virtual Core::Math::Vec3ui GetBrickLayout(size_t LoD, size_t timestep) const{ return Core::Math::Vec3ui(1, 1, 1); };

	virtual Core::Math::Vec4ui IndexTo4D(const BrickKey& key) const { return Core::Math::Vec4ui(1, 1, 1,0); };

    virtual Tuvok::MinMaxBlock MaxMinForKey(const BrickKey&) const{
		MinMaxBlock mm;
		mm.maxScalar = 10000.0f;
		mm.minScalar = 0.0f;
		return mm;

	};
	virtual Core::Math::Vec3ui GetBrickOverlapSize() const { return Core::Math::Vec3ui(0, 0, 0); };

	virtual Core::Math::Vec3f GetBrickExtents(const BrickKey &) const { return Core::Math::Vec3f(1, 1, 1); };

	virtual BrickKey IndexFrom4D(const Core::Math::Vec4ui& four, size_t timestep) const{
		const BrickKey k = BrickKey(1,1,1);
		return k;
	}
	virtual std::pair<double, double> GetRange() const {
		std::pair<double,double> p;
		p.first = 0.0f;
		p.second = 3000.0f;
		return p;
	}
  private:
    bool                            m_bInitialized;
    DataIO::VolumeIO::QVisVolumePtr m_pDataset;
  };
  typedef std::shared_ptr<NoneBrickedQvisIO> NoneBrickedQvisIOPtr;
}

#endif /* defined(__TNG__NonBrickedQvisIO__) */
