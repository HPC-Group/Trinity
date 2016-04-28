#include "gtest/gtest.h"

#include "commands/ProcessingCommands.h"
#include "common/TrinityError.h"
#include "io-base/IONode.h"
#include "io-base/IOSession.h"
#include "io-base/FractalListData.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/ProcessingNode.h"
#include "processing-base/RenderSession.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity;
using namespace mocca::net;

class CmdFactoryTest : public ::testing::Test {
protected:
    CmdFactoryTest() { mocca::net::ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~CmdFactoryTest() { mocca::net::ConnectionFactorySelector::removeAll(); }
};


TEST_F(CmdFactoryTest, VCLCompleteTest) {
    ASSERT_NO_THROW(Vcl::instance());
}

TEST_F(CmdFactoryTest, WrongRequest) {
    StreamingParams streamingParams(2048, 1000);
    InitProcessingSessionCmd::RequestParams requestParams("loopback", VclType::DummyRenderer, "FractalData@3",
                                                          mocca::net::Endpoint("tcp.prefixed:loopback:5678"), streamingParams);
    InitProcessingSessionRequest request(requestParams, 0, 0);
    IONodeCommandFactory factory;
    ASSERT_THROW(factory.createHandler(request, nullptr), TrinityError);
}

TEST_F(CmdFactoryTest, RendererExecTest) {
    mocca::net::Endpoint ioEndpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(ioEndpoint));
    std::unique_ptr<ConnectionAggregator> ioAggregator(
        new ConnectionAggregator(std::move(ioAcceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));

    std::vector<std::unique_ptr<IListData>> listData;
    listData.push_back(mocca::make_unique<FractalListData>());
    IONode ioNode(std::move(ioAggregator), std::move(listData));
    ioNode.start();

    StreamingParams streamingParams(2048, 1000);
    InitProcessingSessionCmd::RequestParams requestParams("loopback", VclType::DummyRenderer, "FractalData@3", ioEndpoint, streamingParams);
    InitProcessingSessionRequest request(requestParams, 0, 0);

    mocca::net::Endpoint processingEndpoint(ConnectionFactorySelector::loopback(), "localhost", "5679");
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> processingAcceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(processingEndpoint));
    std::unique_ptr<ConnectionAggregator> processingAggregator(
        new ConnectionAggregator(std::move(processingAcceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    ProcessingNode processingNode(std::move(processingAggregator));

    ProcessingNodeCommandFactory f;
    auto handler = f.createHandler(request, &processingNode);

    auto result = handler->execute();
    ASSERT_TRUE(result != nullptr);
    auto castedResult = dynamic_cast<InitProcessingSessionReply*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    auto replyParams = castedResult->getParams();
    ASSERT_NE(std::stoi(replyParams.getVisPort()), std::stoi(replyParams.getControlPort()));
    ASSERT_EQ(1, processingNode.getSessions().size());
}