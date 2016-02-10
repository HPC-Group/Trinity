#pragma once
#include <memory>
#include <string>

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/base/Thread.h"

#include "common/IIO.h"
#include "common/Commands.h"

#include "IOCommandFactory.h"




namespace trinity {
namespace io {
    
class IOSession : public mocca::Runnable {

public:
    IOSession(const std::string& protocol, int fileId);
    ~IOSession();
    unsigned int getSid() const;
    common::IIO& getIO();
    
private:
    
    unsigned int m_sid;
    int m_controlPort;
    std::unique_ptr<common::IIO> m_io;
    mocca::net::Endpoint m_controlEndpoint;
    std::unique_ptr<mocca::net::IMessageConnectionAcceptor> m_acceptor;
    std::unique_ptr<mocca::net::IMessageConnection> m_controlConnection;
    common::Vcl m_vcl;
    IOCommandFactory m_factory;
    
    
    static unsigned int m_nextSid;
    static int m_basePort;
    
    void run() override;
};
}
}