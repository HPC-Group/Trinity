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
    auto serialized = Request::createByteArray(request);

    auto result = Request::createFromByteArray(serialized);
    auto castedResult = dynamic_cast<SetIsoValueRequest*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    ASSERT_EQ(3.14f, castedResult->getParams().getIsoValue());
}

TEST_F(RequestTest, MessageSerializationWithBinary) {
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

TEST_F(RequestTest, ByteArraySerializationWithBinary) {
    auto binary = std::make_shared<std::vector<uint8_t>>();
    binary->push_back(0xAA);
    binary->push_back(0xBB);
    binary->push_back(0xCC);
    GetBrickCmd::ReplyParams replyParams(binary, true);
    GetBrickReply reply(replyParams, 0, 0);
    auto serialized = Reply::createByteArray(reply);

    auto result = Reply::createFromByteArray(serialized);
    auto castedResult = dynamic_cast<GetBrickReply*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    ASSERT_EQ(true, castedResult->getParams().getSuccess());
    ASSERT_EQ(*binary, *castedResult->getParams().getBrick());
}