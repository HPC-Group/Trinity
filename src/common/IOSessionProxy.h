#pragma once

#include "common/IIO.h"
#include "commands/CommandInputChannel.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include <memory>


namespace trinity {
namespace common {

class IOSessionProxy : public common::IIO {

public:
    IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint);
    virtual int getLODLevelCount();

private:
    commands::CommandInputChannel m_inputChannel;
    const int m_remoteSid;
};
}
}