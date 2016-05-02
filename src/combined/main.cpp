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
#include "processing-base/ProcessingNode.h"

#include <silverbullet/base/DetectEnv.h>

using namespace trinity;
using namespace mocca::net;

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

mocca::CommandLineParser::Option uvfDataPathOption(std::string& path) {
    mocca::CommandLineParser::Option option;
    option.key = "--uvfPath";
    option.help = "path to uvf files (default: ./)";
    option.callback = [&](const std::string& value) { path = value; };
    return option;
}

mocca::CommandLineParser::Option IoTcpPortOption(int16_t& port) {
    mocca::CommandLineParser::Option option;
    option.key = "--IoTcpPort";
    option.help = "TCP port for IO (default: 6678)";
    option.callback = [&](const std::string& value) { port = std::stoi(value); };
    return option;
}

mocca::CommandLineParser::Option IoWsPortOption(int16_t& port) {
    mocca::CommandLineParser::Option option;
    option.key = "--IoWsPPort";
    option.help = "WebSocket port for IO (default: 6679)";
    option.callback = [&](const std::string& value) { port = std::stoi(value); };
    return option;
}

mocca::CommandLineParser::Option ProcTcpPortOption(int16_t& port) {
    mocca::CommandLineParser::Option option;
    option.key = "--ProcTcpPort";
    option.help = "TCP port for processing (default: 8678)";
    option.callback = [&](const std::string& value) { port = std::stoi(value); };
    return option;
}

mocca::CommandLineParser::Option ProcWsPortOption(int16_t& port) {
    mocca::CommandLineParser::Option option;
    option.key = "--ProcWsPort";
    option.help = "WebSocket port for processing (default: 8679)";
    option.callback = [&](const std::string& value) { port = std::stoi(value); };
    return option;
}

int main(int argc, const char** argv) {
    init();

    int16_t ioTcpPort = 6678;
    int16_t ioWsPort = 6679;
    int16_t procTcpPort = 8678;
    int16_t procWsPort = 8679;

    mocca::CommandLineParser parser;
    std::string uvfDataPath = ".";
    parser.addOption(uvfDataPathOption(uvfDataPath));
    parser.addOption(IoTcpPortOption(ioTcpPort));
    parser.addOption(IoWsPortOption(ioWsPort));
    parser.addOption(ProcTcpPortOption(procTcpPort));
    parser.addOption(ProcWsPortOption(procWsPort));


    try {
        parser.parse(argc, argv);
    } catch (const mocca::CommandLineParser::ParserError& err) {
        std::cerr << err.what() << std::endl;
        std::exit(0);
    }

    Endpoint ioTCPEp(ConnectionFactorySelector::tcpPrefixed(), "localhost", std::to_string(ioTcpPort));
    Endpoint ioWSEp(ConnectionFactorySelector::tcpWebSocket(), "localhost", std::to_string(ioWsPort));
    Endpoint procTCPEp(ConnectionFactorySelector::tcpPrefixed(), "localhost", std::to_string(procTcpPort));
    Endpoint procWSEp(ConnectionFactorySelector::tcpWebSocket(), "localhost", std::to_string(procWsPort));

    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors = mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(
        ConnectionFactorySelector::bind(ioTCPEp), ConnectionFactorySelector::bind(ioWSEp),
        ConnectionFactorySelector::bind(trinity::ioLoopbackEndpoint()));

    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> procAcceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(procTCPEp),
                                                            ConnectionFactorySelector::bind(procWSEp));

    std::unique_ptr<ConnectionAggregator> ioAggregator(
        new ConnectionAggregator(std::move(ioAcceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));

    std::vector<std::unique_ptr<IListData>> listData;
    listData.push_back(mocca::make_unique<FractalListData>());
    listData.push_back(mocca::make_unique<UVFListData>(uvfDataPath));

    IONode ioNode(std::move(ioAggregator), std::move(listData), AbstractNode::ExecutionMode::Combined);
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
