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

class AbstractSession : public mocca::Runnable {

public:
    AbstractSession(const std::string& protocol);
    virtual ~AbstractSession();

    int getSid() const { return m_sid; }
    std::string getControlPort() const { return m_acceptor->localEndpoint()->port; }

private:
    virtual void performThreadSpecificTeardown() {};
    virtual void performThreadSpecificInit() {}
    void run() override;
    virtual std::unique_ptr<ICommandHandler> createHandler(const Request& request) = 0;

private:
    int m_sid;
    std::unique_ptr<mocca::net::IMessageConnectionAcceptor> m_acceptor;
    std::unique_ptr<mocca::net::IMessageConnection> m_controlConnection;

    // todo: one day, we might want to release ids
};
}