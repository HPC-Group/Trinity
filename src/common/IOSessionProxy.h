#pragma once

#include "common/IIO.h"
#include "commands/CommandInputChannel.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include <memory>


namespace trinity {

class IOSessionProxy : public IIO {

public:
    IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint);
    int getLODLevelCount() const override;

private:
    CommandInputChannel m_inputChannel;
    const int m_remoteSid;
};
}