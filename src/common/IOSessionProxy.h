#pragma once
#include <memory>

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "IIO.h"
#include "IProxy.h"

namespace trinity {
namespace common {

class IOSessionProxy : public common::IIO, public common::IProxy {

public:
    IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint);
    virtual int getLODLevelCount();

private:
    const int m_remoteSid;
};
}
}