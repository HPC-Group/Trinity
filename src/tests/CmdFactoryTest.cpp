#include "gtest/gtest.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/base/ContainerTools.h"

#include "commands/ProcessingCommands.h"
#include "commands/ISerialObjectFactory.h"

#include "common/Node.h"
#include "common/ISession.h"

#include "io-base/IOCommandFactory.h"
#include "processing-base/ProcessingCommandFactory.h"

using namespace trinity::common;
using namespace trinity::commands;
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
    ASSERT_NO_THROW(Vcl::instance());
}

TEST_F(CmdFactoryTest, WrongStreamTest) {
    trinity::processing::ProcessingCommandFactory factory;
    std::stringstream s;
    s << "this is not a command";
    ASSERT_THROW(factory.createHandler(s), mocca::Error);
}

TEST_F(CmdFactoryTest, WrongCommandTest) {
    trinity::io::IOCommandFactory factory;
    StreamingParams params(2048, 1000);
    InitProcessingSessionCmd cmd(1, 2, "loopback", VclType::DummyRenderer, 0, "tcp.Prefixed:loopback:5678", params);
    auto obj = ISerialObjectFactory::create();
    cmd.serialize(*obj);
    std::stringstream s;
    obj->writeTo(s);
    ASSERT_THROW(factory.createHandler(s), mocca::Error);
}


TEST_F(CmdFactoryTest, RendererExecTest) {
    StreamingParams params(2048, 1000);
    mocca::net::Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));

    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    std::unique_ptr<trinity::commands::ICommandFactory> factory(new trinity::io::IOCommandFactory);
    trinity::common::Node node(std::move(aggregator), std::move(factory));
    node.start();
    
    InitProcessingSessionCmd cmd(1, 2, "loopback", VclType::DummyRenderer, 0, endpoint.toString(), params);
    auto obj = ISerialObjectFactory::create();
    cmd.serialize(*obj);
    std::stringstream s;
    obj->writeTo(s);

    trinity::processing::ProcessingCommandFactory f;
    auto handler = f.createHandler(s);
    
    handler->execute();
    
    std::unique_ptr<ICommand> rep = handler->getReturnValue();
    ASSERT_TRUE(rep != nullptr);
    
    ICommand* repPtr = rep.release();
    ReplyInitProcessingSessionCmd* castedPtr = dynamic_cast<ReplyInitProcessingSessionCmd*>(repPtr);
    ASSERT_TRUE(castedPtr != nullptr);
    ASSERT_EQ(castedPtr->getType(), VclType::TrinityReturn);
    ASSERT_EQ(castedPtr->getVisPort() - castedPtr->getControlPort(), 1);
}
