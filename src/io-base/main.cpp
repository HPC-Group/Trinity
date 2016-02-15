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
#include "mocca/base/ContainerTools.h"

#include "common/INode.h"
#include "IOCommandFactory.h"

using namespace trinity::io;
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
    signal(SIGINT, exitHandler);
    ConnectionFactorySelector::addDefaultFactories();
}


int main(int argc, char** argv) {
    init();
    
    // endpoints
    Endpoint e1(ConnectionFactorySelector::tcpPrefixed(), "localhost", std::to_string(feTCPPort));
    Endpoint e2(ConnectionFactorySelector::tcpWebSocket(), "localhost", std::to_string(feWSPort));
    
    // connection acceptors for the endpoints
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor>
    (ConnectionFactorySelector::bind(e1), ConnectionFactorySelector::bind(e2));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    std::unique_ptr<trinity::commands::ICommandFactory> factory(new IOCommandFactory);
    trinity::common::INode node(std::move(aggregator), std::move(factory));
    
    node.start();
    while(!exitFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    node.interrupt();
}