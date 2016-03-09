#pragma once

#include "commands/Vcl.h"
#include "common/AbstractSession.h"
#include "common/IIO.h"
#include "common/SessionManager.h"
#include "io-base/IOCommandFactory.h"

#include "mocca/base/Singleton.h"
#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

#include <memory>
#include <string>

namespace trinity {
class IOSession : public AbstractSession {
public:
    IOSession(const std::string& protocol, std::unique_ptr<ICommandFactory> factory, int fileId);

    const IIO& getIO() const { return *m_io; }

private:
    std::unique_ptr<IIO> createIO(int fileId);

private:
    std::unique_ptr<IIO> m_io;
};

using IOSessionManager = mocca::Singleton<SessionManager<IOSession>>;
}
