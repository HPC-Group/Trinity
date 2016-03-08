#pragma once

#include "commands/Vcl.h"
#include "common/IIO.h"
#include "common/ISession.h"
#include "common/SessionManager.h"
#include "io-base/IOCommandFactory.h"

#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/base/Singleton.h"

#include <memory>
#include <string>

namespace trinity {
namespace io {

class IOSession : public common::ISession {

public:
    IOSession(const std::string& protocol, std::unique_ptr<commands::ICommandFactory> factory, int fileId);

    const common::IIO& getIO() const { return *m_io; }

private:
    std::unique_ptr<common::IIO> createIO(int fileId);
    
private:
    std::unique_ptr<common::IIO> m_io;
};

using IOSessionManager = mocca::Singleton<common::SessionManager<IOSession>>;

}
}