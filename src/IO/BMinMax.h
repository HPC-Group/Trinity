#ifndef TUVOK_BMINMAX_H
#define TUVOK_BMINMAX_H

#include "Core/Math/MinMaxBlock.h"
#include "Brick.h"

namespace Tuvok {
class BrickedDataset;

MinMaxBlock minmax_brick(const BrickKey& bk, const BrickedDataset& ds);

}

#endif
