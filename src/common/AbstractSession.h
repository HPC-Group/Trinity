#pragma once

#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

#include <memory>
#include <string>

namespace trinity {

// dmc: are subclasses of AbstractSession really used in a runtime polymorphic way, or do they actually just inherit the implementation?

class AbstractSession : public mocca::Runnable {

public:
    AbstractSession(const std::string& protocol);

    virtual ~AbstractSession();

    virtual int getSid() const { return m_sid; }
    virtual int getControlPort() const { return std::stoi(m_controlEndpoint.port); }

protected: // fixme dmc: protected access to base member -> baaaaad
    static int m_basePort;

private:
    void run() override;
    virtual std::unique_ptr<ICommandHandler> createHandler(const Request& request) const = 0;

private:
    int m_sid;
    static int m_nextSid;

    // connections
    std::unique_ptr<mocca::net::IMessageConnectionAcceptor> m_acceptor;
    mocca::net::Endpoint m_controlEndpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_controlConnection;

    // todo: one day, we might want to release ids
};
}