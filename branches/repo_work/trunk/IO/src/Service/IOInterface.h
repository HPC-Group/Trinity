#ifndef __IOInterface__
#define __IOInterface__

#pragma once

#include <string>
#include <core/Math/Vectors.h>
#include <core/Math/MinMaxBlock.h>
#include <IO/Brick.h>

namespace Tuvok{
	class IOInterface{
		public:
			/*! \brief opens a file
			*
			* This function opens a file using the given filename
			*
			* \param filename is a string value
			* \return unsigned integer. 0 if file could not be opened, else > 0.
			*/
			virtual uint16_t openFile(std::string filename) = 0;
			
			/* \brief closed the dataset
			*
			* This function will close a dataset
			*/
			virtual void closeDataset() = 0;


			/*! \brief returns the max brick size of the dataset
			*
			* \return returns a three dimensional vector of the type unsigned integer.
			*/
			virtual Vec3ui GetMaxBrickSize() = 0;
			/*! \biref returns the max used brick size of the dataset
			*
			* \return returns a three dimensional vector of the type unsigned integer.
			*/
			virtual Vec3ui GetMaxUsedBrickSizes() = 0;

			/*! \biref this function returns a MaxMinBlock for a brick inside the dataset
			*
			* A MinMaxBlock contains information about the scalar and gradient of a brick
			* more information can be found inside <core/Math/MinMaxBlock.h>
			*
			* \param BirckKey < timestep + LOD + 1D brick index >
			* \return A MinMaxBlock which stores the min/max values of the brick
			*/
			virtual MinMaxBlock MaxMinForKey(const BrickKey&) = 0;

			/*! \brief returns the count of LOD levels
			* \return unsigned integer
			*/
			virtual unsigned GetLODLevelCount() = 0;

			/*! \brief reurns the number of timestemps
			* \return 64 bit unsigned integer value 
			*/
			virtual uint64_t GetNumberOfTimesteps() = 0;
			/*! \brief dimensions of the dataset
			* 
			* This function will return the size of the dataset given a specifiy LOD and timestemp
			* \return 64 bit three dimensional vector of unsigned integer values
			*/
			virtual Vec3ui64 GetDomainSize(const size_t lod = 0, const size_t ts = 0) = 0;
			/*! \brief Scale factor of the dataset
			*
			* The returned vector determines the scaling factor for axis
			* \return three dimensional vector of type double
			*/
			virtual Vec3d GetScale() = 0;
			/*! \brief returns the overlap of a brick
			*
			* \return three dimensional vector of type unsigned integer
			*/
			virtual Vec3ui GetBrickOverlapSize() = 0;
			/*! \brief Returns the largest brick
			*/
			virtual size_t GetLargestSingleBrickLOD(size_t ts) = 0;
			virtual Vec3ui GetBrickVoxelCounts(const BrickKey&) = 0;
			virtual Vec3f GetBrickExtents(const BrickKey &) = 0;
			virtual Vec3ui GetBrickLayout(size_t LoD, size_t timestep) = 0;

			virtual unsigned GetBitWidth() = 0;
			virtual uint64_t GetComponentCount() = 0;
			virtual bool GetIsSigned() = 0;
			virtual bool GetIsFloat() = 0;
			virtual uint64_t GetTotalBrickCount() = 0;

			virtual Core::Math::Vec4ui IndexTo4D(const BrickKey& key) = 0;
			virtual BrickKey IndexFrom4D(const Vec4ui& four, size_t timestep) = 0;

			virtual Core::Math::Vec2f GetRange() = 0;

			virtual bool GetBrick(const BrickKey&, std::vector< uint8_t>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector<uint16_t>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector<uint32_t>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector<  int8_t>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector< int16_t>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector< int32_t>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector<   float>&) = 0;
			virtual bool GetBrick(const BrickKey&, std::vector<  double>&) = 0;

};
}
#endif /* defined(__IOInterface__) */