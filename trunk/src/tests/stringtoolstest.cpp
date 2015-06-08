#include "gtest/gtest.h"

namespace {


    class StringToolsTest : public ::testing::Test {
    protected:
        StringToolsTest() {
            // You can do set-up work for each test here.
        }

        virtual ~StringToolsTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }


    };

    TEST_F(StringToolsTest, StringTokenizerTest) {
        {
            ASSERT_EQ(4, 4);
        }
    }


}  // namespace
