#include "gtest/gtest.h"

#include "commands/Request.h"
#include "commands/ProcessingCommands.h"

using namespace trinity;

class RequestTest : public ::testing::Test {
protected:
    RequestTest() {}
    virtual ~RequestTest() {}
};

TEST_F(RequestTest, GetType) {
    SetIsoValueCmd::RequestParams requestParams(3.14f);
    SetIsoValueRequest request(requestParams, 0, 0);
    ASSERT_EQ(VclType::SetIsoValue, request.getType());
}

TEST_F(RequestTest, Serialization) {
    SetIsoValueCmd::RequestParams requestParams(3.14f);
    SetIsoValueRequest request(requestParams, 0, 0);
    auto serialized = Request::createByteArray(request);

    auto result = Request::createFromByteArray(serialized);
    auto castedResult = dynamic_cast<SetIsoValueRequest*>(result.get());
    ASSERT_TRUE(castedResult != nullptr);
    ASSERT_EQ(3.14f, castedResult->getParams().getIsoValue());
}