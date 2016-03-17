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
#include "common/ProxyUtils.h"
#include "common/IONodeProxy.h"

using namespace mocca::net;

std::atomic<bool> exitFlag{false};

std::unique_ptr<trinity::ProcessingNodeProxy> processingNode;
std::unique_ptr<trinity::IONodeProxy> ioNode;


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

    // endpoints for processing and io nodes
    // hint: do not implement any cmd line parsers for that. They are already available
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), "localhost", "8678");
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");


    ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
    auto datalist = ioNode->listFiles("0");
    LINFO("listing dir 0: ");
    for(const auto& data : datalist) {
        LINFO(data.toString());
    }
    datalist = ioNode->listFiles("1");
    LINFO("listing dir 1: ");
    for(const auto& data : datalist) {
        LINFO(data.toString());
    }
    datalist = ioNode->listFiles("2");
    LINFO("listing dir 2: ");
    for(const auto& data : datalist) {
        LINFO(data.toString());
    }
    
    
    processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));

    // the file id will be available after implementing the listdata command
    std::string fileId = "0";
    trinity::StreamingParams params(1024, 768);
    auto renderer = processingNode->initRenderer(trinity::VclType::GridLeaper, fileId, endpointIO, params);

    // sending commands
    renderer->setIsoValue(22);

    //wait for 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // receiving images


    while (!exitFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto visStream = renderer->getVisStream();
        auto frameNullable = visStream->get();
        if (frameNullable.isNull())
            LINFO("no frame arrived yet");
        else {
            LINFO("frame arrived!");
            auto frame = frameNullable.release();            LINFO("content: " + frame.read(frame.size()));
        }
    };
}
