#pragma once

#include "silverbullet/dataio/base/Brick.h"
#include "silverbullet/math/MinMaxBlock.h"
#include "silverbullet/math/Vectors.h"
#include <string>


/**
 OLD FILE - use it only as an inspiration
 the new IO interface is in common/IIO.h
 */


using namespace Tuvok;
namespace Trinity {
namespace IO {

enum Semantic { Scalar, Vector, Color };

enum ValueType { T_FLOAT, T_DOUBLE, T_UINT8, T_UINT16, T_UINT32, T_UINT64, T_INT8, T_INT16, T_INT32, T_INT64 };

class IOInterface {
public:
    /*! \brief returns the max brick size of the dataset
     *
     * \return returns a three dimensional vector of the type unsigned integer.
     */
    virtual Core::Math::Vec3ui64 getMaxBrickSize() const = 0;

    /*! \brief returns the max used brick size of the dataset
     *
     * \return returns a three dimensional vector of the type unsigned integer.
     */
    virtual Core::Math::Vec3ui64 getMaxUsedBrickSizes() const = 0;

    /*! \biref this function returns a MaxMinBlock for a brick inside the dataset
     *
     * A MinMaxBlock contains information about the scalar and gradient of a brick
     * more information can be found inside <core/Math/MinMaxBlock.h>
     *
     * \param BirckKey < timestep + LOD + 1D brick index >
     * \return A MinMaxBlock which stores the min/max values of the brick
     */
    virtual MinMaxBlock maxMinForKey(const BrickKey&) const = 0;

    /*! \brief returns the count of LOD levels
     * \return unsigned integer
     */
    virtual unsigned getLODLevelCount() const = 0;

    /*! \brief returns the number of timestemps
     * \return 64 bit unsigned integer value
     */
    virtual uint64_t getNumberOfTimesteps() const = 0;

    /*! \brief dimensions of the dataset
     *
     * This function will return the size of the dataset given a specifiy LOD and timestemp
     * \return 64 bit three dimensional vector of unsigned integer values
     */
    virtual Core::Math::Vec3ui64 getDomainSize(const uint64_t lod = 0, const uint64_t modality = 0, const uint64_t ts = 0) const = 0;

    /*! \brief Transformation factor of the dataset (replaces getScale)
     *
     * The returned vector determines the scaling factor for axis
     * \return three dimensional vector of type double
     */
    virtual Core::Math::Mat4d getTransformation(const uint64_t modality) const = 0;

    /*! \brief returns the overlap of a brick
     *
     * \return three dimensional vector of type unsigned integer
     */
    virtual Core::Math::Vec3ui getBrickOverlapSize() const = 0;

    /*! \brief Returns the largest brick
     */
    virtual uint64_t getLargestSingleBrickLOD(const uint64_t modality, uint64_t ts) const = 0;
    virtual Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const = 0;
    virtual Core::Math::Vec3f getBrickExtents(const BrickKey&) const = 0;
    virtual Core::Math::Vec3ui getBrickLayout(uint64_t LoD, const uint64_t modality, uint64_t timestep) const = 0;

    /*! \brief Replaces getBitWidth(), getIsSigned(), getIsFloat()
     */
    virtual ValueType getType() const = 0;

    virtual uint64_t getModalityCount() const = 0;

    virtual uint64_t getComponentCount(uint64_t modality) const = 0;

    virtual Semantic getSemantic(uint64_t modality) const = 0;

    virtual std::string getUserDefinedSemantic(uint64_t modality) const = 0;

    virtual Core::Math::Vec2f getRange(const uint64_t modality) const = 0;

    virtual uint64_t getTotalBrickCount() const = 0;

    virtual bool getBrick(const BrickKey&, std::vector<uint8_t>&) const = 0;
};
}
}