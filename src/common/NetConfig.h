#pragma once
#include <memory>
#include <chrono>

namespace trinity {
namespace common {

static const std::chrono::milliseconds TIMEOUT_REPLY(10000);
static const int RENDERSESSION_BASE_PORT = 5999;
static const int VISRECEIVER_BASE_PORT = 6999;
}
}