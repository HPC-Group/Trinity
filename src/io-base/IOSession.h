#pragma once
#include <memory>
#include <string>

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/base/Thread.h"

#include "common/IIO.h"
#include "commands/Vcl.h"

#include "IOCommandFactory.h"
#include "common/ISession.h"




namespace trinity {
namespace io {
    
class IOSession : public common::ISession {

public:
    IOSession(const std::string& protocol,
              std::unique_ptr<commands::ICommandFactory> factory,
              int fileId);
    ~IOSession();
    common::IIO& getIO() { return *m_io; }
    
private:
    std::unique_ptr<common::IIO> m_io;
    std::unique_ptr<common::IIO>
    createIO(int fileId, const commands::VclType& ioType = commands::VclType::DummyIO);
};
}
}