#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"

#include "ProcessingNode.h"

static std::chrono::milliseconds receiveTimeout(50);
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

    NetworkServiceLocator::provideAll();
    trinity::ProcessingNode tcpNode(Endpoint(NetworkServiceLocator::tcpPrefixed(),
                                          std::to_string(feTCPPort)));
     
    trinity::ProcessingNode wsNode(Endpoint(NetworkServiceLocator::tcpWebSocket(),
                                          std::to_string(feWSPort)));

    tcpNode.start();
    wsNode.start();
    while(!exitFlag) {
        
    }
    tcpNode.interrupt();
    wsNode.interrupt();
}