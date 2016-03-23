#pragma once

#include "commands/ISerializable.h"

#include <algorithm>
#include <cfloat>

/// Stores minimum and maximum data for a block of data.
class MinMaxBlock : public trinity::SerializableTemplate<MinMaxBlock> {
public:
    double minScalar;
    double maxScalar;
    double minGradient;
    double maxGradient;

  MinMaxBlock() :
   minScalar(DBL_MAX),
   maxScalar(-FLT_MAX),
   minGradient(DBL_MAX),
   maxGradient(-FLT_MAX)
  {}

  MinMaxBlock(double _minScalar, double _maxScalar,
              double _minGradient, double _maxGradient) :
   minScalar(_minScalar),
   maxScalar(_maxScalar),
   minGradient(_minGradient),
   maxGradient(_maxGradient)
  {}

  void merge(const MinMaxBlock& other) {
    minScalar = std::min(minScalar, other.minScalar);
    maxScalar = std::max(maxScalar, other.maxScalar);
    minGradient = std::min(minGradient, other.minGradient);
    maxGradient = std::max(maxGradient, other.maxGradient);
  }

  void serialize(trinity::ISerialWriter& writer) const;
  void deserialize(const trinity::ISerialReader& reader);

  bool equals(const MinMaxBlock& other) const;
  std::string toString() const;
};

bool operator==(const MinMaxBlock& lhs, const MinMaxBlock& rhs);
std::ostream& operator<<(std::ostream& os, const MinMaxBlock& obj);
