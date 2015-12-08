#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/WSNetworkService.h"
#include "mocca/net/MoccaNetworkService.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/TCPNetworkAddress.h"
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

    NetworkServiceLocator::provideService(std::make_shared<MoccaNetworkService>(
    std::unique_ptr<IPhysicalNetworkService>(new TCPNetworkService())));

    
    std::unique_ptr<IPhysicalNetworkService> tcpService(new TCPNetworkService());
    
    std::unique_ptr<IProtocolNetworkService> wsService(new WSNetworkService(std::move(tcpService)));
    
    NetworkServiceLocator::provideService(std::move(wsService));

    
    trinity::ProcessingNode tcpNode(Endpoint(MoccaNetworkService::protocolStatic(),
                                          TCPNetworkService::transportStatic(),
                                          std::to_string(feTCPPort)));
     
    
    trinity::ProcessingNode wsNode(Endpoint(WSNetworkService::protocolStatic(),
                                            TCPNetworkService::transportStatic(),
                                          std::to_string(feWSPort)));

    //std::thread t1(&trinity::ProcessingNode::listen, &wsNode);
    tcpNode.listen();
    //wsNode.listen();
}