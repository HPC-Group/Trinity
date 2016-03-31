#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/base/ByteArray.h"
#include "mocca/base/ContainerTools.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/HTMLLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "io-base/IONode.h"
#include "processing-base/ProcessingNode.h"

#include <silverbullet/base/DetectEnv.h>

using namespace trinity;
using namespace mocca::net;

static int ioTCPPort = 6678;
static int ioWSPort = 6679;
static int procTCPPort = 8678;
static int procWSPort = 8679;

std::atomic<bool> exitFlag{false};

void exitHandler(int s) {
    std::cout << "Trinity exit on signal " << std::to_string(s) << std::endl;
    exitFlag = true;
}

void init() {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto console = new mocca::ConsoleLog();
    LogMgr.addLog(console);
#ifdef DETECTED_OS_LINUX
    auto web = new mocca::HTMLLog("processinglog.html");
    LogMgr.addLog(web);
#endif
    signal(SIGINT, exitHandler);
    ConnectionFactorySelector::addDefaultFactories();
}

int main(int argc, char** argv) {
    init();

    Endpoint ioTCPEp(ConnectionFactorySelector::tcpPrefixed(), "localhost", std::to_string(ioTCPPort));
    Endpoint ioWSEp(ConnectionFactorySelector::tcpWebSocket(), "localhost", std::to_string(ioWSPort));
    Endpoint procTCPEp(ConnectionFactorySelector::tcpPrefixed(), "localhost", std::to_string(procTCPPort));
    Endpoint procWSEp(ConnectionFactorySelector::tcpWebSocket(), "localhost", std::to_string(procWSPort));

    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors = mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(
        ConnectionFactorySelector::bind(ioTCPEp), ConnectionFactorySelector::bind(ioWSEp),
        ConnectionFactorySelector::bind(trinity::ioLoopbackEndpoint()));

    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> procAcceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(procTCPEp),
                                                            ConnectionFactorySelector::bind(procWSEp));

    std::unique_ptr<ConnectionAggregator> ioAggregator(
        new ConnectionAggregator(std::move(ioAcceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    IONode ioNode(std::move(ioAggregator), AbstractNode::ExecutionMode::Combined);
    ioNode.start();

    std::unique_ptr<ConnectionAggregator> procAggregator(
        new ConnectionAggregator(std::move(procAcceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    ProcessingNode procNode(std::move(procAggregator), AbstractNode::ExecutionMode::Combined);
    procNode.start();

    while (!exitFlag) {
        try {
            procNode.rethrowException();
        } catch (const std::runtime_error& err) {
            LERROR("(p) Error occurred: " << err.what());
        }

        try {
            ioNode.rethrowException();
        } catch (const std::runtime_error& err) {
            LERROR("(io) Error occurred: " << err.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    ioNode.join();
    procNode.join();
}
