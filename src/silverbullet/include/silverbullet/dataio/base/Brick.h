/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2009 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/
/**
  \file    Brick.h
  \author  Tom Fogal
           SCI Institute
           University of Utah
*/
#pragma once
#ifndef TUVOK_BRICK_H
#define TUVOK_BRICK_H

#include "silverbullet/base/StdTuvokDefines.h"

#include "silverbullet/math/Vectors.h"

#include "commands/ISerializable.h"

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

using Core::Math::Vec3f;
using Core::Math::Vec3ui;

namespace trinity {

/// Datasets are organized as a set of bricks, stored in a hash table.  A key
/// into this table consists of an LOD index plus a brick index.
/// An element in the table contains
/// brick metadata, but no data; to obtain the data one must query the dataset.

struct BrickKey : public SerializableTemplate<BrickKey> {
    uint64_t modality;
    uint64_t timestep;
    uint64_t lod;
    uint64_t index;

    BrickKey() = default;
    BrickKey(uint64_t m, uint64_t t, uint64_t l, uint64_t i) : modality(m), timestep(t), lod(l), index(i) {}
    
    bool equals(const BrickKey& other) const;
    std::string toString() const;

    void serialize(ISerialWriter& writer) const;
    void deserialize(const ISerialReader& reader);
};

bool operator==(const BrickKey& lhs, const BrickKey& rhs);
std::ostream& operator<<(std::ostream& os, const BrickKey& obj);

struct BrickMD {
  Vec3f center; ///< center of the brick, in world coords
  Vec3f extents; ///< width/height/depth of the brick.
  Vec3ui  n_voxels; ///< number of voxels, per dimension.
};
struct BKeyHash : std::unary_function<BrickKey, std::size_t> {
  std::size_t operator()(const BrickKey& bk) const {
    size_t ts    = std::hash<size_t>()(bk.timestep);
    size_t h_lod = std::hash<size_t>()(bk.lod);
    size_t brick = std::hash<size_t>()(bk.index);
    size_t seed = h_lod;
    seed ^= brick + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= ts + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};
typedef std::unordered_map<BrickKey, BrickMD, BKeyHash> BrickTable;

} // namespace Tuvok

#endif // TUVOK_BRICK_H
