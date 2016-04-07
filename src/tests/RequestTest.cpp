#include "gtest/gtest.h"

#include "commands/Request.h"
#include "commands/ProcessingCommands.h"
#include "commands/IOCommands.h"

using namespace trinity;

class RequestTest : public ::testing::Test {
protected:
    RequestTest() {}
    virtual ~RequestTest() {}
};

TEST_F(RequestTest, GetType) {
    SetIsoValueCmd::RequestParams requestParams(2, 3.14f);
    SetIsoValueRequest request(requestParams, 0, 0);
    ASSERT_EQ(VclType::SetIsoValue, request.getType());
}

TEST_F(RequestTest, Serialization) {
    SetIsoValueCmd::RequestParams requestParams(2, 3.14f);
    SetIsoValueRequest request(requestParams, 0, 0);
    auto serialized = Request::createMessage(request);

    auto result = Request::createFromMessage(serialized);
    auto castedResult = dynamic_cast<SetIsoValueRequest*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    ASSERT_EQ(3.14f, castedResult->getParams().getIsoValue());
}

TEST_F(RequestTest, GetBrickWithMessage) {
    auto binary = std::make_shared<std::vector<uint8_t>>();
    binary->push_back(0xAA);
    binary->push_back(0xBB);
    binary->push_back(0xCC);
    GetBrickCmd::ReplyParams replyParams(binary, true);
    GetBrickReply reply(replyParams, 0, 0);
    auto serialized = Reply::createMessage(reply);

    auto result = Reply::createFromMessage(serialized);
    auto castedResult = dynamic_cast<GetBrickReply*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    ASSERT_EQ(true, castedResult->getParams().getSuccess());
    ASSERT_EQ(*binary, *castedResult->getParams().getBrick());
}

TEST_F(RequestTest, GetBricksWithMessage) {
    auto binary1 = std::make_shared<std::vector<uint8_t>>();
    binary1->push_back(0xAA);
    binary1->push_back(0xBB);
    binary1->push_back(0xCC);

    auto binary2 = std::make_shared<std::vector<uint8_t>>();
    binary2->push_back(0xDD);
    binary2->push_back(0xEE);

    GetBricksCmd::ReplyParams replyParams({ binary1, binary2 }, true);
    GetBricksReply reply(replyParams, 0, 0);
    auto serialized = Reply::createMessage(reply);

    auto result = Reply::createFromMessage(serialized);
    auto castedResult = dynamic_cast<GetBricksReply*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    ASSERT_EQ(true, castedResult->getParams().getSuccess());
    ASSERT_EQ(2, castedResult->getParams().getResult().size());
    ASSERT_EQ(*binary1, *castedResult->getParams().getResult()[0]);
    ASSERT_EQ(*binary2, *castedResult->getParams().getResult()[1]);
}