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

#include "ProcessingNode.h"

using namespace trinity::processing;

static int feTCPPort = 5678;
static int feWSPort = 5679;
std::atomic<bool> exitFlag{false};


void exitHandler(int s) {
    std::cout << "Trinity exit on signal " << std::to_string(s) << std::endl;
    exitFlag = true;
}


void initLogging() {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto console = new mocca::ConsoleLog();
    LogMgr.addLog(console);
}

using namespace mocca::net;

int main(int argc, char** argv) {
    initLogging();
    signal(SIGINT, exitHandler);

    ConnectionFactorySelector::addDefaultFactories();
    
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
    
    ProcessingNode node(std::move(aggregator));

    node.start();
    while(!exitFlag) {
        
    }
    node.interrupt();
}