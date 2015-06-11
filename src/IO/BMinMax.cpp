#include <algorithm>
#include "BMinMax.h"
#include <core/Math/MinMaxBlock.h>
#include "BrickedDataset.h"
#include <tools/DebugOutHandler.h>


namespace {
  template<typename T> Tuvok::MinMaxBlock mm(const Tuvok::BrickKey& bk,
                                             const Tuvok::BrickedDataset& ds) {
    std::vector<T> data(ds.GetMaxBrickSize().volume());
    ds.GetBrick(bk, data);
    auto mmax = std::minmax_element(data.begin(), data.end());
    return Tuvok::MinMaxBlock(*mmax.first, *mmax.second, DBL_MAX, -FLT_MAX);
  }
}

namespace Tuvok {

MinMaxBlock minmax_brick(const BrickKey& bk, const BrickedDataset& ds) {
  // identify type (float, etc)
  const unsigned size = ds.GetBitWidth() / 8;
  assert(ds.GetComponentCount() == 1);
  const bool sign = ds.GetIsSigned();
  const bool fp = ds.GetIsFloat();

  // giant if-block to simply call the right compile-time function w/ knowledge
  // we only know at run-time.
  if(!sign && !fp && size == 1) {
    return mm<uint8_t>(bk, ds);
  } else if(!sign && !fp && size == 2) {
    return mm<uint16_t>(bk, ds);
  } else if(!sign && !fp && size == 4) {
    return mm<uint32_t>(bk, ds);
  } else if(sign && !fp && size == 1) {
    return mm<int8_t>(bk, ds);
  } else if(sign && !fp && size == 2) {
    return mm<int16_t>(bk, ds);
  } else if(sign && !fp && size == 4) {
    return mm<int32_t>(bk, ds);
  } else if(sign && fp && size == 4) {
    return mm<float>(bk, ds);
  } else {
    IVDA_ERROR("unsupported type.");
    assert(false);
  }
  return MinMaxBlock();
}

}