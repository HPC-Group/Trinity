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

TEST_F(IOCommandsTest, GetMaxUsedBrickSizesCmd) {
    {
        GetMaxUsedBrickSizesCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        Core::Math::Vec3ui64 vec(1, 2, 3);
        GetMaxUsedBrickSizesCmd::ReplyParams target(vec);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetMaxUsedBrickSizesReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getMaxUsedBrickSizes()).Times(1).WillOnce(Return(Core::Math::Vec3ui64(1, 2, 3)));

    GetMaxUsedBrickSizesCmd::RequestParams requestParams;
    GetMaxUsedBrickSizesRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetMaxUsedBrickSizesHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui64(1, 2, 3), reply.getParams().getMaxUsedBrickSizes());
}

TEST_F(IOCommandsTest, BrickKey) {
    BrickKey target{ 1, 2, 3, 4 };
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}

TEST_F(IOCommandsTest, MinMaxBlock) {
    MinMaxBlock target{ 1.0, 2.0, 3.0, 4.0 };
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}

TEST_F(IOCommandsTest, MaxMinForKeyCmd) {
    {
        BrickKey key(1, 2, 3, 4);
        MaxMinForKeyCmd::RequestParams target(key);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        MinMaxBlock minMax(1.0, 2.0, 3.0, 4.0);
        MaxMinForKeyCmd::ReplyParams target(minMax);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, MaxMinForKeyReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), maxMinForKey(_)).Times(1).WillOnce(Return(MinMaxBlock(1.0, 2.0, 3.0, 4.0)));

    MaxMinForKeyCmd::RequestParams requestParams(BrickKey(1, 2, 3, 4));
    MaxMinForKeyRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<MaxMinForKeyHdl>(request, session.get());
    ASSERT_EQ(MinMaxBlock(1.0, 2.0, 3.0, 4.0), reply.getParams().getMinMaxBlock());
}

TEST_F(IOCommandsTest, GetNumberOfTimestepsCmd) {
    {
        GetNumberOfTimestepsCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        uint64_t numberOfTimesteps = 42;
        GetNumberOfTimestepsCmd::ReplyParams target(numberOfTimesteps);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetNumberOfTimestepsReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getNumberOfTimesteps()).Times(1).WillOnce(Return(42));

    GetNumberOfTimestepsCmd::RequestParams requestParams;
    GetNumberOfTimestepsRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetNumberOfTimestepsHdl>(request, session.get());
    ASSERT_EQ(42, reply.getParams().getNumberOfTimesteps());
}

TEST_F(IOCommandsTest, GetDomainSizeCmd) {
    {
        GetDomainSizeCmd::RequestParams target(1, 2, 3);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetDomainSizeCmd::ReplyParams target(Core::Math::Vec3ui64(4, 5, 6));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetDomainSizeReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getDomainSize(_,_,_)).Times(1).WillOnce(Return(Core::Math::Vec3ui64(4, 5, 6)));

    GetDomainSizeCmd::RequestParams requestParams;
    GetDomainSizeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetDomainSizeHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui64(4, 5, 6), reply.getParams().getDomainSize());
}

TEST_F(IOCommandsTest, GetTransformationCmd) {
    {
        GetTransformationCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        Core::Math::Mat4d transformation(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
        GetTransformationCmd::ReplyParams target(transformation);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetTransformationReqRep) {
    auto session = createMockSession();
    Core::Math::Mat4d transformation(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getTransformation()).Times(1).WillOnce(Return(transformation));

    GetTransformationCmd::RequestParams requestParams;
    GetTransformationRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetTransformationHdl>(request, session.get());
    ASSERT_EQ(transformation, reply.getParams().getTransformation());
}

TEST_F(IOCommandsTest, GetBrickOverlapSizeCmd) {
    {
        GetBrickOverlapSizeCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetBrickOverlapSizeCmd::ReplyParams target(Core::Math::Vec3ui(4, 5, 6));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetBrickOverlapSizeReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getBrickOverlapSize()).Times(1).WillOnce(Return(Core::Math::Vec3ui(4, 5, 6)));

    GetBrickOverlapSizeCmd::RequestParams requestParams;
    GetBrickOverlapSizeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetBrickOverlapSizeHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui(4, 5, 6), reply.getParams().getOverlapSize());
}