#include "common/ProxyUtils.h"

namespace trinity {
namespace common {
bool connectInputChannel(const commands::CommandInputChannel& inputChannel) {
    const int maxAttempts = 3;
    const int reconnectInSec = 5;
    bool connected = false;
    int attempt = 0;

    while (!connected && attempt < maxAttempts) {
        if (!inputChannel.connect()) {
            LINFO("reconnecting to io in " << std::to_string(reconnectInSec) << " seconds");
            std::this_thread::sleep_for(std::chrono::seconds(reconnectInSec));
        } else {
            return true;
        }
        ++attempt;
    }
    return false;
}
}
}
