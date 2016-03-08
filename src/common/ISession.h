#pragma once
#include <memory>
#include <string>

#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

#include "commands/ICommandFactory.h"


namespace trinity {

// dmc: are subclasses of ISession really used in a runtime polymorphic way, or do they actually just inherit the implementation?

class ISession : public mocca::Runnable {

public:
    ISession(const std::string& protocol, std::unique_ptr<ICommandFactory> factory);

    virtual ~ISession();

    virtual int getSid() const { return m_sid; }
    virtual int getControlPort() const { return std::stoi(m_controlEndpoint.port); }

protected:
    int m_sid;
    static int m_nextSid;
    static int m_basePort;

    // connections
    std::unique_ptr<mocca::net::IMessageConnectionAcceptor> m_acceptor;
    mocca::net::Endpoint m_controlEndpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_controlConnection;

    std::unique_ptr<ICommandFactory> m_factory;

    void run() override;


    // todo: one day, we might want to release ids
};
}