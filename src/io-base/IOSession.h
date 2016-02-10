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
    unsigned int getSid() const;
    common::IIO& getIO();
    
private:
    
    unsigned int m_sid;
    std::unique_ptr<common::IIO> m_io;
    
    
    static unsigned int m_nextSid;
    static int m_basePort;
};
}
}