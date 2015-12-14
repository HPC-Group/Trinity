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

void initLogging() {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto console = new mocca::ConsoleLog();
    LogMgr.addLog(console);
}

using namespace mocca::net;

int main(int argc, char** argv) {
    initLogging();

    NetworkServiceLocator::provideAll();

    trinity::ProcessingNode tcpNode(
        Endpoint(NetworkServiceLocator::tcpPrefixed(), std::to_string(feTCPPort)));

    trinity::ProcessingNode wsNode(
        Endpoint(NetworkServiceLocator::tcpPrefixed(), std::to_string(feWSPort)));

    // std::thread t1(&trinity::ProcessingNode::listen, &wsNode);
    tcpNode.listen();
    // wsNode.listen();
}