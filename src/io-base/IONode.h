#pragma once

#include "common/Node.h"
#include "io-base/IOCommandFactory.h"

namespace trinity {
namespace io {
using IONode = trinity::common::Node<IOCommandFactory>;
}
}
