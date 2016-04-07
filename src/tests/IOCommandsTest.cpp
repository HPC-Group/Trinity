#include "gtest/gtest.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/TransferFunction1D.h"
#include "commands/Vcl.h"
#include "common/TrinityError.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOCommandsHandler.h"

#include "tests/IOMock.h"
#include "tests/TestUtils.h"

#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity;
using namespace ::testing;


class IOCommandsTest : public ::testing::Test {
protected:
    IOCommandsTest() { mocca::net::ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~IOCommandsTest() { mocca::net::ConnectionFactorySelector::removeAll(); }

    std::unique_ptr<IOSession> createMockSession() { return mocca::make_unique<IOSession>("loopback", mocca::make_unique<IOMock>()); }
};

TEST_F(IOCommandsTest, InitIOSessionCmd) {
    {
        InitIOSessionCmd::RequestParams target("protocol", "1");
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
        GetLODLevelCountCmd::RequestParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetLODLevelCountCmd::ReplyParams target(4711);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetLODLevelCountReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getLODLevelCount(42)).Times(1).WillOnce(Return(4711));

    GetLODLevelCountCmd::RequestParams requestParams(42);
    GetLODLevelCountRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetLODLevelCountHdl>(request, session.get());
    ASSERT_EQ(4711, reply.getParams().getLODLevelCount());
}

TEST_F(IOCommandsTest, IOData) {
    {
        IOData target("name", "4711", IOData::DataType::Dataset);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesCmd) {
    {
        ListFilesCmd::RequestParams target("FractalData@3");
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        IOData data1("name1", "FractalData@3", IOData::DataType::Dataset);
        IOData data2("name2", "FractalData@2", IOData::DataType::Directory);
        std::vector<IOData> dataVec{data1, data2};
        ListFilesCmd::ReplyParams target(dataVec);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesReqRep) {
    auto ioNode = mocca::make_unique<IONode>();
    {
        ListFilesCmd::RequestParams requestParams("FractalData@3");
        ListFilesRequest request(requestParams, 1, 2);
        auto reply = trinity::testing::handleRequest<ListFilesHdl>(request, ioNode.get());
        ASSERT_EQ(0, reply.getParams().getIOData().size());
    }
    {
        ListFilesCmd::RequestParams requestParams("invalid"); // invalid directory ID
        ListFilesRequest request(requestParams, 1, 2);
        ASSERT_THROW(trinity::testing::handleRequest<ListFilesHdl>(request, ioNode.get()), TrinityError);
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
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getMaxUsedBrickSizes())
        .Times(1)
        .WillOnce(Return(Core::Math::Vec3ui64(1, 2, 3)));

    GetMaxUsedBrickSizesCmd::RequestParams requestParams;
    GetMaxUsedBrickSizesRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetMaxUsedBrickSizesHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui64(1, 2, 3), reply.getParams().getMaxUsedBrickSizes());
}

TEST_F(IOCommandsTest, BrickKey) {
    BrickKey target{1, 2, 3, 4};
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}

TEST_F(IOCommandsTest, MinMaxBlock) {
    MinMaxBlock target{1.0, 2.0, 3.0, 4.0};
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
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), maxMinForKey(BrickKey(1, 2, 3, 4)))
        .Times(1)
        .WillOnce(Return(MinMaxBlock(1.0, 2.0, 3.0, 4.0)));

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
        GetDomainSizeCmd::RequestParams target(1, 2);
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
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getDomainSize(1, 2)).Times(1).WillOnce(Return(Core::Math::Vec3ui64(4, 5, 6)));

    GetDomainSizeCmd::RequestParams requestParams(1, 2);
    GetDomainSizeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetDomainSizeHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui64(4, 5, 6), reply.getParams().getDomainSize());
}

TEST_F(IOCommandsTest, GetTransformationCmd) {
    {
        GetTransformationCmd::RequestParams target(42);
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
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getTransformation(42)).Times(1).WillOnce(Return(transformation));

    GetTransformationCmd::RequestParams requestParams(42);
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

TEST_F(IOCommandsTest, GetLargestSingleBrickLODCmd) {
    {
        GetLargestSingleBrickLODCmd::RequestParams target(23);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetLargestSingleBrickLODCmd::ReplyParams target(23);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetLargestSingleBrickLODReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getLargestSingleBrickLOD(17)).Times(1).WillOnce(Return(42));

    GetLargestSingleBrickLODCmd::RequestParams requestParams(17);
    GetLargestSingleBrickLODRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetLargestSingleBrickLODHdl>(request, session.get());
    ASSERT_EQ(42, reply.getParams().getLargestSingleBrickLOD());
}

TEST_F(IOCommandsTest, GetBrickVoxelCountsCmd) {
    {
        GetBrickVoxelCountsCmd::RequestParams target(BrickKey(1, 2, 3, 4));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetBrickVoxelCountsCmd::ReplyParams target(Core::Math::Vec3ui(5, 6, 7));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetBrickVoxelCountsReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getBrickVoxelCounts(BrickKey(1, 2, 3, 4)))
        .Times(1)
        .WillOnce(Return(Core::Math::Vec3ui(5, 6, 7)));

    GetBrickVoxelCountsCmd::RequestParams requestParams(BrickKey(1, 2, 3, 4));
    GetBrickVoxelCountsRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetBrickVoxelCountsHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui(5, 6, 7), reply.getParams().getBrickVoxelCounts());
}

TEST_F(IOCommandsTest, GetBrickExtentsCmd) {
    {
        GetBrickExtentsCmd::RequestParams target(BrickKey(1, 2, 3, 4));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetBrickExtentsCmd::ReplyParams target(Core::Math::Vec3f(5.0, 6.0, 7.0));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetBrickExtentsReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getBrickExtents(BrickKey(1, 2, 3, 4)))
        .Times(1)
        .WillOnce(Return(Core::Math::Vec3f(5.0, 6.0, 7.0)));

    GetBrickExtentsCmd::RequestParams requestParams(BrickKey(1, 2, 3, 4));
    GetBrickExtentsRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetBrickExtentsHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3f(5.0, 6.0, 7.0), reply.getParams().getBrickExtents());
}

TEST_F(IOCommandsTest, GetBrickLayoutCmd) {
    {
        GetBrickLayoutCmd::RequestParams target(1, 2);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetBrickLayoutCmd::ReplyParams target(Core::Math::Vec3ui(4, 5, 6));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetBrickLayoutReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getBrickLayout(1, 2)).Times(1).WillOnce(Return(Core::Math::Vec3ui(4, 5, 6)));

    GetBrickLayoutCmd::RequestParams requestParams(1, 2);
    GetBrickLayoutRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetBrickLayoutHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec3ui(4, 5, 6), reply.getParams().getBrickLayout());
}

TEST_F(IOCommandsTest, GetModalityCountCmd) {
    {
        GetModalityCountCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetModalityCountCmd::ReplyParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetModalityCountReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getModalityCount()).Times(1).WillOnce(Return(23));

    GetModalityCountCmd::RequestParams requestParams;
    GetModalityCountRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetModalityCountHdl>(request, session.get());
    ASSERT_EQ(23, reply.getParams().getModalityCount());
}

TEST_F(IOCommandsTest, GetComponentCountCmd) {
    {
        GetComponentCountCmd::RequestParams target(23);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetComponentCountCmd::ReplyParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetComponentCountReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getComponentCount(23)).Times(1).WillOnce(Return(42));

    GetComponentCountCmd::RequestParams requestParams(23);
    GetComponentCountRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetComponentCountHdl>(request, session.get());
    ASSERT_EQ(42, reply.getParams().getComponentCount());
}

TEST_F(IOCommandsTest, GetRangeCmd) {
    {
        GetRangeCmd::RequestParams target(23);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetRangeCmd::ReplyParams target(Core::Math::Vec2f(1.0, 2.0));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetRangeReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getRange(23)).Times(1).WillOnce(Return(Core::Math::Vec2f(1.0, 2.0)));

    GetRangeCmd::RequestParams requestParams(23);
    GetRangeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetRangeHdl>(request, session.get());
    ASSERT_EQ(Core::Math::Vec2f(1.0, 2.0), reply.getParams().getRange());
}

TEST_F(IOCommandsTest, GetTotalBrickCountCmd) {
    {
        GetTotalBrickCountCmd::RequestParams target(23);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetTotalBrickCountCmd::ReplyParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetTotalBrickCountReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getTotalBrickCount(23)).Times(1).WillOnce(Return(42));

    GetTotalBrickCountCmd::RequestParams requestParams(23);
    GetTotalBrickCountRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetTotalBrickCountHdl>(request, session.get());
    ASSERT_EQ(42, reply.getParams().getTotalBrickCount());
}

TEST_F(IOCommandsTest, GetBrickCmd) {
    {
        GetBrickCmd::RequestParams target(BrickKey(1, 2, 3, 4));
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        std::initializer_list<uint8_t> data = {0x12, 0x34, 0x56, 0x78, 0x9A};
        auto binary = std::make_shared<std::vector<uint8_t>>(data);
        GetBrickCmd::ReplyParams target(binary, true);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetBrickReqRep) {
    auto session = createMockSession();
    auto brick = std::make_shared<std::vector<uint8_t>>();
    *brick = { 0x12, 0x34, 0x56, 0x78, 0x9A };
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getBrick(BrickKey(1, 2, 3, 4), _))
        .Times(1)
        .WillOnce(Return(brick));

    GetBrickCmd::RequestParams requestParams(BrickKey(1, 2, 3, 4));
    GetBrickRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetBrickHdl>(request, session.get());
    ASSERT_EQ(*brick, *reply.getParams().getBrick());
}

TEST_F(IOCommandsTest, GetTypeCmd) {
    {
        GetTypeCmd::RequestParams target(23);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetTypeCmd::ReplyParams target(IIO::ValueType::T_INT32);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetTypeReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getType(23)).Times(1).WillOnce(Return(IIO::ValueType::T_INT32));

    GetTypeCmd::RequestParams requestParams(23);
    GetTypeRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetTypeHdl>(request, session.get());
    ASSERT_EQ(IIO::ValueType::T_INT32, reply.getParams().getValueType());
}

TEST_F(IOCommandsTest, GetSemanticCmd) {
    {
        GetSemanticCmd::RequestParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetSemanticCmd::ReplyParams target(IIO::Semantic::Color);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetSemanticReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getSemantic(42)).Times(1).WillOnce(Return(IIO::Semantic::Color));

    GetSemanticCmd::RequestParams requestParams(42);
    GetSemanticRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetSemanticHdl>(request, session.get());
    ASSERT_EQ(IIO::Semantic::Color, reply.getParams().getSemantic());
}

TEST_F(IOCommandsTest, GetDefault1DTransferFunctionCountCmd) {
    {
        GetDefault1DTransferFunctionCountCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetDefault1DTransferFunctionCountCmd::ReplyParams target(4711);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetDefault1DTransferFunctionCountReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getDefault1DTransferFunctionCount()).Times(1).WillOnce(Return(4711));

    GetDefault1DTransferFunctionCountCmd::RequestParams requestParams;
    GetDefault1DTransferFunctionCountRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetDefault1DTransferFunctionCountHdl>(request, session.get());
    ASSERT_EQ(4711, reply.getParams().getCount());
}

TEST_F(IOCommandsTest, GetDefault2DTransferFunctionCountCmd) {
    {
        GetDefault2DTransferFunctionCountCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetDefault2DTransferFunctionCountCmd::ReplyParams target(4711);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetDefault2DTransferFunctionCountReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getDefault2DTransferFunctionCount()).Times(1).WillOnce(Return(4711));

    GetDefault2DTransferFunctionCountCmd::RequestParams requestParams;
    GetDefault2DTransferFunctionCountRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetDefault2DTransferFunctionCountHdl>(request, session.get());
    ASSERT_EQ(4711, reply.getParams().getCount());
}

TEST_F(IOCommandsTest, Get1DHistogramCmd) {
    {
        Get1DHistogramCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        Get1DHistogramCmd::ReplyParams target(std::vector<uint64_t>{1, 2, 3, 4});
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, Get1DHistogramReqRep) {
    auto session = createMockSession();
    std::vector<uint64_t> histogram{1, 2, 3, 4};
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), get1DHistogram()).Times(1).WillOnce(Return(histogram));

    Get1DHistogramCmd::RequestParams requestParams;
    Get1DHistogramRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<Get1DHistogramHdl>(request, session.get());
    ASSERT_EQ(histogram, reply.getParams().getHistogram());
}

TEST_F(IOCommandsTest, Get2DHistogramCmd) {
    {
        Get2DHistogramCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        Get2DHistogramCmd::ReplyParams target(std::vector<uint64_t>{1, 2, 3, 4});
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, Get2DHistogramReqRep) {
    auto session = createMockSession();
    std::vector<uint64_t> histogram{1, 2, 3, 4};
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), get2DHistogram()).Times(1).WillOnce(Return(histogram));

    Get2DHistogramCmd::RequestParams requestParams;
    Get2DHistogramRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<Get2DHistogramHdl>(request, session.get());
    ASSERT_EQ(histogram, reply.getParams().getHistogram());
}

TEST_F(IOCommandsTest, GetUserDefinedSemanticCmd) {
    {
        GetUserDefinedSemanticCmd::RequestParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        GetUserDefinedSemanticCmd::ReplyParams target("UserSemantic");
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetUserDefinedSemanticReqRep) {
    auto session = createMockSession();
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getUserDefinedSemantic(42)).Times(1).WillOnce(Return("UserSemantic"));

    GetUserDefinedSemanticCmd::RequestParams requestParams(42);
    GetUserDefinedSemanticRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetUserDefinedSemanticHdl>(request, session.get());
    ASSERT_EQ("UserSemantic", reply.getParams().getSemantic());
}

TEST_F(IOCommandsTest, TransferFunction1D) {
    TransferFunction1D target;
    target.set({0x11, 0x22, 0x33, 0xAA, 0xBB, 0xCC});
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target.getRAWData(), result.getRAWData());
    ASSERT_EQ(target.getNonZeroLimits(), result.getNonZeroLimits());
}

TEST_F(IOCommandsTest, GetDefault1DTransferFunctionCmd) {
    {
        GetDefault1DTransferFunctionCmd::RequestParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        TransferFunction1D function;
        function.set({ 0x11, 0x22, 0x33, 0xAA, 0xBB, 0xCC });
        GetDefault1DTransferFunctionCmd::ReplyParams target(function);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetDefault1DTransferFunctionReqRep) {
    auto session = createMockSession();
    TransferFunction1D function;
    function.set({ 0x11, 0x22, 0x33, 0xAA, 0xBB, 0xCC });
    EXPECT_CALL(static_cast<const IOMock&>(session->getIO()), getDefault1DTransferFunction(42)).Times(1).WillOnce(Return(function));

    GetDefault1DTransferFunctionCmd::RequestParams requestParams(42);
    GetDefault1DTransferFunctionRequest request(requestParams, 1, 2);
    auto reply = trinity::testing::handleRequest<GetDefault1DTransferFunctionHdl>(request, session.get());
    ASSERT_EQ(function, reply.getParams().getFunction());
}