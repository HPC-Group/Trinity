#include "gtest/gtest.h"
#include "logging/logmanager.h"

// used logs
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

using namespace ghoul::logging;

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
            /*
            LogManager::initialize(LogManager::LogLevel::Info, true);
            
            Log* html = new HTMLLog("blubbla.html");
            Log* text = new TextLog("blubbla.txt");
            Log* console = new ConsoleLog();
            LogMgr.addLog(html);
            LogMgr.addLog(text);
            LogMgr.addLog(console);
            
            LINFOC("bla log", "blub");
            
            ASSERT_EQ(4, 4);
             */
        }
    }


}  // namespace
