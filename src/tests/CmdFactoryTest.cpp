#include "gtest/gtest.h"

#include "mocca/net/ConnectionFactorySelector.h"

#include "common/ICommand.h"
#include "common/Commands.h"
#include "common/ICommandHandler.h"
#include "common/ProcessingNodeCmds.h"
#include "common/ISerialObjectFactory.h"

#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/ProcessingNodeCmdsHdl.h"
#include "processing-base/RenderSessionManager.h"

#include "io-base/IONode.h"

#include "mocca/base/ContainerTools.h"

using namespace trinity::common;
using namespace trinity::processing;
using namespace mocca::net;

class CmdFactoryTest : public ::testing::Test {
protected:

      CmdFactoryTest() {
        mocca::net::ConnectionFactorySelector::addDefaultFactories();
    }
    
    virtual ~CmdFactoryTest() {
        SessionManagerSingleton::instance()->endAllSessions();
        mocca::net::ConnectionFactorySelector::removeAll();
    }
};


TEST_F(CmdFactoryTest, VCLCompleteTest) {
    ASSERT_NO_THROW(Vcl());
}

TEST_F(CmdFactoryTest, RendererExecTest) {
    StreamingParams params(2048, 1000);
    mocca::net::Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));

    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::io::IONode node(std::move(aggregator));
    node.start();
    
    InitRendererCmd cmd(1, 2, "loopback", VclType::DummyRenderer, 0, endpoint.toString(), params);
    auto obj = ISerialObjectFactory::create();
    cmd.serialize(*obj);
    std::stringstream s;
    obj->writeTo(s);

    ProcessingCommandFactory factory;
    auto handler = factory.createHandler(s);
    
    handler->execute();
    
    std::unique_ptr<ICommand> rep = handler->getReturnValue();
    ASSERT_TRUE(rep != nullptr);
    
    ICommand* repPtr = rep.release();
    ReplyInitRendererCmd* castedPtr = dynamic_cast<ReplyInitRendererCmd*>(repPtr);
    ASSERT_TRUE(castedPtr != nullptr);
    ASSERT_EQ(castedPtr->getType(), VclType::TrinityReturn);
    ASSERT_EQ(castedPtr->getVisPort() - castedPtr->getControlPort(), 1);
    
    // todo: execute, get return value
    
}