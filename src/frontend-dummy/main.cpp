#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "ProcessingPrx.h"
#include "common/Commands.h"

static int reconnectInSec = 5;
std::atomic<bool> exitFlag{false};
std::unique_ptr<trinity::ProcessingPrx> processingNode;

void exitHandler(int s) {
    std::cout << "Trinity exit on signal " << std::to_string(s) << std::endl;
    exitFlag = true;
}

void init() {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto console = new mocca::ConsoleLog();
    LogMgr.addLog(console);
    signal(SIGINT, exitHandler);
}

using namespace mocca::net;

int main(int argc, char** argv) {
    init();

    ConnectionFactorySelector::addDefaultFactories();

    Endpoint processingEndpoint(ConnectionFactorySelector::tcpPrefixed(), "localhost:5678");

    processingNode =
        std::unique_ptr<trinity::ProcessingPrx>(new trinity::ProcessingPrx(processingEndpoint));

    bool connected = false;

    while (!connected && !exitFlag) {
        connected = processingNode->connect();
        if (!connected) {
            LINFO("reconnecting in " << std::to_string(reconnectInSec) << " seconds");
            std::this_thread::sleep_for(std::chrono::seconds(reconnectInSec));
        }
    }
    try {
        processingNode->spawnRenderer(trinity::vcl::DUMMY_RENDERER);
    } catch (const mocca::Error& err) {
        LERROR(err.what());
    }

    while (!exitFlag) {
    };
}