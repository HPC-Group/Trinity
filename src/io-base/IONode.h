#pragma once

#include "common/Node.h"
#include "io-base/IOCommandFactory.h"

namespace trinity {
using IONode = trinity::Node<IOCommandFactory>;
}
