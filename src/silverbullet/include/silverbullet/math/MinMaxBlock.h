#pragma once

#include <algorithm>
#include <limits>

/// Stores minimum and maximum data for a block of data.
class MinMaxBlock {
public:
    double minScalar;
    double maxScalar;
    double minGradient;
    double maxGradient;

  MinMaxBlock() :
   minScalar(std::numeric_limits<float>::max()),
   maxScalar(-std::numeric_limits<float>::max()),
   minGradient(std::numeric_limits<float>::max()),
   maxGradient(-std::numeric_limits<float>::max())
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
};