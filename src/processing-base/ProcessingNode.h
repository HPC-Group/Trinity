#pragma once

#include "common/Node.h"
#include "processing-base/ProcessingCommandFactory.h"

namespace trinity {
namespace processing {
using ProcessingNode = trinity::common::Node<ProcessingCommandFactory>;
}
}
