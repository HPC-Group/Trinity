#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/base/CommandLineParser.h"
#include "mocca/base/ContainerTools.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/HTMLLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "io-base/FractalListData.h"
#include "io-base/IONode.h"
#include "io-base/UVFListData.h"

#include <silverbullet/base/DetectEnv.h>

using namespace trinity;
using namespace mocca::net;

static int feTCPPort = 6678;
static int feWSPort = 6679;
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
    auto web = new mocca::HTMLLog("iolog.html");
    LogMgr.addLog(web);
#endif
    signal(SIGINT, exitHandler);
    ConnectionFactorySelector::addDefaultFactories();
}

mocca::CommandLineParser::Option uvfDataPathOption(std::string& path) {
    mocca::CommandLineParser::Option option;
    option.key = "--uvfPath";
    option.help = "path to uvf files (default: ./)";
    option.callback = [&](const std::string& value) { path = value; };
    return option;
}

int main(int argc, const char** argv) {
    init();

    mocca::CommandLineParser parser;
    std::string uvfDataPath = ".";
    parser.addOption(uvfDataPathOption(uvfDataPath));

    try {
        parser.parse(argc, argv);
    } catch (const mocca::CommandLineParser::ParserError& err) {
        std::cerr << err.what() << std::endl;
        std::exit(0);
    }

    // endpoints
    Endpoint e1(ConnectionFactorySelector::tcpPrefixed(), "localhost", std::to_string(feTCPPort));
    Endpoint e2(ConnectionFactorySelector::tcpWebSocket(), "localhost", std::to_string(feWSPort));

    // connection acceptors for the endpoints
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(e1), ConnectionFactorySelector::bind(e2));

    std::unique_ptr<ConnectionAggregator> aggregator(
        new ConnectionAggregator(std::move(acceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));

    std::vector<std::unique_ptr<IListData>> listData;
    listData.push_back(mocca::make_unique<FractalListData>());
    listData.push_back(mocca::make_unique<UVFListData>(uvfDataPath));

    IONode node(std::move(aggregator), std::move(listData));
    node.start();

    while (!exitFlag) {
        try {
            node.rethrowException();
        } catch (const std::runtime_error& err) {
            LERROR("(io) Error occurred: " << err.what());
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    node.join();
}