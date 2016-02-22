#pragma once
#include <memory>

#include "common/VisStream.h"
#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"


// subject of refactoring
namespace trinity {
namespace frontend {

class VisStreamReceiver : public mocca::Runnable {

public:
    VisStreamReceiver(const mocca::net::Endpoint, std::shared_ptr<common::VisStream>);
    ~VisStreamReceiver();

    void startStreaming();
    void endStreaming();

private:
    std::shared_ptr<common::VisStream> m_visStream;
    void run() override;

    mocca::net::Endpoint m_endpoint;
    std::unique_ptr<mocca::net::IMessageConnection> m_connection;
};
}
}