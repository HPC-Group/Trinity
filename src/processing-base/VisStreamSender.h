#pragma once
#include <memory>

#include "common/VisStream.h"
#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

// subject of refactoring
namespace trinity {
// todo sender and receiver do share much code for now
// however, this will change when videocodec will be added
class VisStreamSender : public mocca::Runnable {
public:
    VisStreamSender(const mocca::net::Endpoint, std::shared_ptr<VisStream>);
    ~VisStreamSender();

    void startStreaming();
    void endStreaming();

    std::string getPort() const;
    std::shared_ptr<VisStream> getStream() const { return m_visStream; }

private:
    std::shared_ptr<VisStream> m_visStream;
    void run() override;

    std::unique_ptr<mocca::net::IMessageConnectionAcceptor> m_acceptor;
    std::unique_ptr<mocca::net::IMessageConnection> m_connection;
};
}