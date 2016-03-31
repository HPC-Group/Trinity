#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "common/IONodeProxy.h"
#include "frontend-base/ProcessingNodeProxy.h"

class ConnectionSingleton{
public:
    static ConnectionSingleton& getInstance(){
         static ConnectionSingleton    instance;
         return instance;
    }

    trinity::IONodeProxy& ioNode();
    trinity::ProcessingNodeProxy& processingNode();

    void connectIO(const std::string& hostname, const std::string& port);
    void connectProcessing(const std::string& hostname, const std::string& port);

private:
    ConnectionSingleton();
    ~ConnectionSingleton();

    std::unique_ptr<trinity::ProcessingNodeProxy>   _processingNode;
    std::unique_ptr<trinity::IONodeProxy>           _ioNode;
};
