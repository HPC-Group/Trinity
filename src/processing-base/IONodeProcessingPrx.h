#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include "common/Commands.h"
#include "common/CommandInputChannel.h"
#include "IOSessionPrx.h"


namespace trinity {
namespace processing {
    
class IONodeProcessingPrx {
    
public:
    /// see mocca::net::Endpoint for details, supports remote and local calls
    IONodeProcessingPrx(const mocca::net::Endpoint&);
    ~IONodeProcessingPrx();
    
    /// true on success. Does not throw any errors. Disconnect happens in dtor
    bool connect();
    
    std::unique_ptr<IOSessionPrx> initIO(int fileId);

    
private:
    common::CommandInputChannel m_inputChannel;
    common::IDGenerator m_ridGen;
    common::Vcl m_vcl;


};
}
}