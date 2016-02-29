#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "commands/Vcl.h"

#include "ProcessingNodeProxy.h"
#include "common/IONodeProxy.h"

using namespace mocca::net;

static int reconnectInSec = 5;
std::atomic<bool> exitFlag{false};

std::unique_ptr<trinity::frontend::ProcessingNodeProxy> processingNode;
std::unique_ptr<trinity::common::IONodeProxy> ioNode;


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
    ConnectionFactorySelector::addDefaultFactories();
}

void connectLoop(trinity::common::IProxy& proxy) {
    bool connected = false;

    while (!connected && !exitFlag) {
        connected = proxy.connect();
        if (!connected) {
            LINFO("reconnecting to io in " << std::to_string(reconnectInSec) << " seconds");
            std::this_thread::sleep_for(std::chrono::seconds(reconnectInSec));
        }
    }
}

int main(int argc, char** argv) {

    init();

    // endpoints for processing and io nodes
    // hint: do not implement any cmd line parsers for that. They are already available
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), "localhost", "5678");
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");


    ioNode = std::unique_ptr<trinity::common::IONodeProxy>(new trinity::common::IONodeProxy(endpointIO));
    connectLoop(*ioNode);

    processingNode = std::unique_ptr<trinity::frontend::ProcessingNodeProxy>(new trinity::frontend::ProcessingNodeProxy(endpoint));
    connectLoop(*processingNode);

    // the file id will be available after implementing the listdata command
    int fileId = 0;
    trinity::commands::StreamingParams params(1024, 768);

    auto renderer = processingNode->initRenderer(trinity::commands::VclType::DummyRenderer, fileId, endpointIO, params);
    renderer->connect();

    // sending commands
    renderer->setIsoValue(22);

    // receiving images
    auto visStream = renderer->getVisStream();
    auto frame = visStream->get();
    if (!frame)
        LINFO("no frame arrived yet");

    while (!exitFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };
}
