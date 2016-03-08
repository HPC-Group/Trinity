#pragma once

#include "common/IOSessionProxy.h"
#include "commands/IOCommands.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

namespace trinity {

class IONodeProxy {
public:
    IONodeProxy(const mocca::net::Endpoint& ep);

    std::unique_ptr<IOSessionProxy> initIO(int fileId);
    std::vector<IOData> listFiles(int32_t dirID);

private:
    CommandInputChannel m_inputChannel;
};
}