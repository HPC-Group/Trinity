#include "gtest/gtest.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/ProcessingCommandsHandler.h"

#include "tests/RendererMock.h"
#include "tests/TestUtils.h"

#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity;
using namespace ::testing;


class ProcessingCommandsTest : public ::testing::Test {
protected:
    ProcessingCommandsTest() { mocca::net::ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~ProcessingCommandsTest() { mocca::net::ConnectionFactorySelector::removeAll(); }

    std::unique_ptr<RenderSession> createMockSession() { return mocca::make_unique<RenderSession>("loopback", mocca::make_unique<RendererMock>()); }
};


TEST_F(ProcessingCommandsTest, StreamParams) {
    StreamingParams target(2048, 1000);
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}

TEST_F(ProcessingCommandsTest, InitProcessingSessionCmd) {
    {
        StreamingParams streamingParams(2048, 1000);
        InitProcessingSessionCmd::RequestParams target("tcp.prefixed", VclType::DummyRenderer, "FractalData@3", mocca::net::Endpoint("loopback", "local", "mq"), streamingParams);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        InitProcessingSessionCmd::ReplyParams target("123", "456");
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, SetIsoValueCmd) {
    SetIsoValueCmd::RequestParams target(17, 3.14f);
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}

TEST_F(ProcessingCommandsTest, SetRenderModeCmd) {
    {
        SetRenderModeCmd::RequestParams target(IRenderer::ERenderMode::RM_CLEARVIEW);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, SupportsRenderModeCmd) {
    {
        SupportsRenderModeCmd::RequestParams target(IRenderer::ERenderMode::RM_CLEARVIEW);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        SupportsRenderModeCmd::ReplyParams target(true);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, SupportsRenderModeReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<RendererMock&>(session->getRenderer()), supportsRenderMode(IRenderer::ERenderMode::RM_CLEARVIEW)).Times(1).WillOnce(Return(true));

    SupportsRenderModeCmd::RequestParams requestParams(IRenderer::ERenderMode::RM_CLEARVIEW);
    SupportsRenderModeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<SupportsRenderModeHdl>(request, session.get());
    ASSERT_EQ(true, reply.getParams().getResult());
}

TEST_F(ProcessingCommandsTest, SetActiveModalityCmd) {
    {
        SetActiveModalityCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, GetActiveModalityCmd) {
    {
        GetActiveModalityCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetActiveModalityCmd::ReplyParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, GetActiveModalityReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<RendererMock&>(session->getRenderer()), getActiveModality()).Times(1).WillOnce(Return(42));

    GetActiveModalityCmd::RequestParams requestParams;
    GetActiveModalityRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetActiveModalityHdl>(request, session.get());
    ASSERT_EQ(42, reply.getParams().getModality());
}

TEST_F(ProcessingCommandsTest, SetActiveTimestepCmd) {
    {
        SetActiveTimestepCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, GetActiveTimestepCmd) {
    {
        GetActiveTimestepCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetActiveTimestepCmd::ReplyParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, GetActiveTimestepReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<RendererMock&>(session->getRenderer()), getActiveTimestep()).Times(1).WillOnce(Return(42));

    GetActiveTimestepCmd::RequestParams requestParams;
    GetActiveTimestepRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetActiveTimestepHdl>(request, session.get());
    ASSERT_EQ(42, reply.getParams().getTimestep());
}