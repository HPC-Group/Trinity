#include <sstream>
#include <thread>

#include "mocca/base/Error.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"

#include "ProcessingNodePrx.h"

#include "common/ErrorCmd.h"
#include "common/NetConfig.h"
#include "common/InitRendererCmd.h"

using namespace trinity::frontend;
using namespace trinity::common;

//static std::chrono::milliseconds receiveTimeout(50);

ProcessingNodePrx::~ProcessingNodePrx() {}

ProcessingNodePrx::ProcessingNodePrx(mocca::net::Endpoint endpoint) :
m_endpoint(endpoint) {}


bool ProcessingNodePrx::connect() {
    
    try {
        m_mainChannel = mocca::net::ConnectionFactorySelector::connect(m_endpoint);
        
    } catch (const mocca::net::ConnectFailedError&) {
        LWARNING("(f) no connection to processing  at \"" << m_endpoint << "\": ");
        return false;
    }
    
    LINFO("(f) successfully connected to processing");
    return true;
}


std::unique_ptr<RendererPrx> ProcessingNodePrx::initRenderer(const VclType& type,
                                                             const StreamingParams& params) {
    
    // creating the cmd that will initialize a remote renderer of the given type
    InitRendererCmd cmd(0, m_ridGen.nextID(), m_endpoint.protocol, type, params);
    std::stringstream s1, s2;
    cmd.serialize(s1);
    
    // command is sent to processing node
    m_mainChannel->send(std::move(mocca::ByteArray()<< s1.str()));
    
    auto byteArray = m_mainChannel->receive(trinity::common::TIMEOUT_REPLY);
    
    if(byteArray.isEmpty()) {
        throw mocca::Error("cannot init renderer, no reply arrived", __FILE__, __LINE__);
    }
    
    // the reply is of type ReplyInitRendererCmd
    std::string rep = byteArray.read(byteArray.size());
    LINFO("(f) cmd : " << s1.str() << "; reply: " << rep);
    
    s2 << rep;
    
    // determining result type
    std::string typeString;
    s2 >> typeString;
    VclType resultType;
    
    try {
        resultType = m_vcl.toType(typeString);
    } catch (const mocca::Error& err) {
        throw mocca::Error("init renderer, result ill-formed", __FILE__, __LINE__);
    }
    
    if(resultType == VclType::TrinityError) {  // error arrived
        ErrorCmd error;
        error.deserialize(s2);
        throw mocca::Error("init renderer: error was returned: " + error.printError(), __FILE__, __LINE__);
    }
    
    if(resultType != VclType::TrinityReturn)  // sth strange arrived
        throw mocca::Error("init renderer: result not of type RET or ERR", __FILE__, __LINE__);
    
    ReplyInitRendererCmd replyCmd;
    replyCmd.deserialize(s2);
    
    mocca::net::Endpoint controlEndpoint(m_endpoint.protocol,
                                         "localhost",
                                         std::to_string(replyCmd.getControlPort()));
    
    
    mocca::net::Endpoint visEndpoint(m_endpoint.protocol,
                                     "localhost",
                                     std::to_string(replyCmd.getVisPort()));

    
    std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(params);
    LINFO("(f) creating render proxy for " << controlEndpoint);
    
    std::unique_ptr<RendererPrx> ren(new RendererPrx(stream,
                                                     std::move(controlEndpoint),
                                                     std::move(visEndpoint),
                                                     replyCmd.getSid()));
    return ren;

}