#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/IConnectionListener.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/AbstractConnection.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"

#include "ProcessingNode.h"


static std::chrono::milliseconds receiveTimeout(50);
static int fePort = 5678;



void initLogging() {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto console = new mocca::ConsoleLog();
    LogMgr.addLog(console);
}


using namespace mocca::net;

int main(int argc, char** argv)
{
    initLogging();
    
    
    NetworkServiceLocator locator;
    locator.provideService(std::make_shared<TCPNetworkService>());
    
    trinity::ProcessingNode node(Endpoint(TCPNetworkService::transportStatic(),
                                          std::to_string(fePort)));
    
    node.listen();
}