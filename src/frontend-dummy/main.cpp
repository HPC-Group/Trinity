#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/MoccaNetworkService.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "ProcessingPrx.h"

std::atomic<bool> exitFlag{false};
static int reconnectInSec = 5;

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

    NetworkServiceLocator::provideService(std::make_shared<MoccaNetworkService>(
        std::unique_ptr<IPhysicalNetworkService>(new TCPNetworkService())));

    Endpoint processingEndpoint(MoccaNetworkService::protocolStatic(),
                                TCPNetworkService::transportStatic(), "localhost:5678");

    trinity::ProcessingPrx processingNode(processingEndpoint);

    bool connected = false;

    while (!connected && !exitFlag) {
        connected = processingNode.connect();
        if (!connected) {
            LINFO("reconnecting in " << std::to_string(reconnectInSec) << " seconds");
            std::this_thread::sleep_for(std::chrono::seconds(reconnectInSec));
        }
    }

    processingNode.spawnRenderer();

    while (!exitFlag) {
    };
}