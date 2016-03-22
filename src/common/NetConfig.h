#pragma once
#include <memory>
#include <chrono>

namespace trinity {

static const std::chrono::milliseconds TIMEOUT_REPLY(20000);
static const int RENDERSESSION_BASE_PORT = 5999;
static const int VISRECEIVER_BASE_PORT = 6999;
}