#include <thread>
#include "IOSessionPrx.h"

#include "mocca/log/LogManager.h"

using namespace trinity::processing;
using namespace trinity::common;

IOSessionPrx::IOSessionPrx(const int sid, const mocca::net::Endpoint& ioEndpoint) :
m_controlEndpoint(ioEndpoint),
m_inputChannel(m_controlEndpoint),
m_sid(sid) {
}
IOSessionPrx::~IOSessionPrx() {
    
}
bool IOSessionPrx::connect() {
    LINFO("(p) connecing to remote io at " << m_controlEndpoint);
    return m_inputChannel.connect();
}

int IOSessionPrx::getLODLevelCount() const {
    /*
    SetIsoValueCmd cmd(m_sid, m_ridGen.nextID(), value);
    
    // send cmd, receive reply
    m_inputChannel.sendCommand(cmd);
     */
    return 0;
}