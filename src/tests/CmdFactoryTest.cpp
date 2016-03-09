#include "gtest/gtest.h"

#include "commands/ProcessingCommands.h"
#include "processing-base/RenderSession.h"
#include "io-base/IOSession.h"
#include "io-base/IONode.h"
#include "processing-base/ProcessingCommandFactory.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity;
using namespace mocca::net;

class CmdFactoryTest : public ::testing::Test {
protected:
    CmdFactoryTest() { mocca::net::ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~CmdFactoryTest() {
        trinity::IOSessionManager::instance()->endAllSessions();
        trinity::RenderSessionManager::instance()->endAllSessions();
        mocca::net::ConnectionFactorySelector::removeAll();
    }
};


TEST_F(CmdFactoryTest, VCLCompleteTest) {
    ASSERT_NO_THROW(Vcl::instance());
}

TEST_F(CmdFactoryTest, WrongRequest) {
    StreamingParams streamingParams(2048, 1000);
    InitProcessingSessionCmd::RequestParams requestParams("loopback", VclType::DummyRenderer, 0, "tcp.Prefixed:loopback:5678", streamingParams);
    InitProcessingSessionRequest request(requestParams, 0, 0);
    trinity::IONodeCommandFactory factory;
    ASSERT_THROW(factory.createHandler(request), TrinityError);
}

TEST_F(CmdFactoryTest, RendererExecTest) {
    mocca::net::Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");

    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(endpoint));
    std::unique_ptr<ConnectionAggregator> aggregator(
        new ConnectionAggregator(std::move(acceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));

    IONode node(std::move(aggregator));
    node.start();

    StreamingParams streamingParams(2048, 1000);
    InitProcessingSessionCmd::RequestParams requestParams("loopback", VclType::DummyRenderer, 0, endpoint.toString(), streamingParams);
    InitProcessingSessionRequest request(requestParams, 0, 0);

    trinity::ProcessingNodeCommandFactory f;
    auto handler = f.createHandler(request);

    auto result = handler->execute();
    ASSERT_TRUE(result != nullptr);
    auto castedResult = dynamic_cast<InitProcessingSessionReply*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    auto replyParams = castedResult->getParams();
    ASSERT_EQ(replyParams.getVisPort() - replyParams.getControlPort(), 1);
}