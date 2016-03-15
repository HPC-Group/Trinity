#include "gtest/gtest.h"

#include "common/TrinityError.h"
#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOCommandsHandler.h"

#include "tests/TestUtils.h"
#include "tests/IOMock.h"

#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity;
using namespace ::testing;


class IOCommandsTest : public ::testing::Test {
protected:
    IOCommandsTest() {
        mocca::net::ConnectionFactorySelector::addDefaultFactories();
    }

    virtual ~IOCommandsTest() {
        mocca::net::ConnectionFactorySelector::removeAll();
    }

    std::unique_ptr<IOSession> createMockSession() {
        return mocca::make_unique<IOSession>("loopback", mocca::make_unique<IOMock>());
    }
};

TEST_F(IOCommandsTest, InitIOSessionCmd) {
    {
        InitIOSessionCmd::RequestParams target("protocol", 42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        InitIOSessionCmd::ReplyParams target("4711");
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetLODLevelCountCmd) {
    {
        GetLODLevelCountCmd::ReplyParams target(4711);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, IOData) {
    {
        IOData target("name", 4711, IOData::DataType::Dataset);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesCmd) {
    {
        ListFilesCmd::RequestParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        IOData data1("name1", 1, IOData::DataType::Dataset);
        IOData data2("name2", 2, IOData::DataType::Directory);
        std::vector<IOData> dataVec{ data1, data2 };
        ListFilesCmd::ReplyParams target(dataVec);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesReqRep) {
    {
        ListFilesCmd::RequestParams requestParams(0);
        ListFilesRequest request(requestParams, 1, 2);
        auto reply = trinity::testing::handleRequest<ListFilesHdl>(request, static_cast<IONode*>(nullptr));
        ASSERT_EQ(2, reply.getParams().getIOData().size());
    }
    {
        ListFilesCmd::RequestParams requestParams(42); // invalid directory ID
        ListFilesRequest request(requestParams, 1, 2);
        ASSERT_THROW(trinity::testing::handleRequest<ListFilesHdl>(request, static_cast<IONode*>(nullptr)), TrinityError);
    }
}

TEST_F(IOCommandsTest, GetMaxBrickSizeCmd) {
    {
        GetMaxBrickSizeCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        Core::Math::Vec3ui64 vec(1, 2, 3);
        GetMaxBrickSizeCmd::ReplyParams target(vec);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetMaxBrickSizeReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getMaxBrickSize()).Times(1).WillOnce(Return(Core::Math::Vec3ui64(1, 2, 3)));

    GetMaxBrickSizeCmd::RequestParams requestParams;
    GetMaxBrickSizeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetMaxBrickSizeHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui64(1, 2, 3), reply.getParams().getMaxBrickSize());
}
