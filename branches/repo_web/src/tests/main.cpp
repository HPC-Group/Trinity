#include "gmock/gmock.h"

/*
#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"
*/

//using namespace ghoul::logging;

int main(int argc, char** argv) {

	/*
    LogManager::initialize(LogManager::LogLevel::Debug, true);

    Log* html = new HTMLLog("testlog.html");
    Log* text = new TextLog("testlog.txt");
    Log* console = new ConsoleLog();
    LogMgr.addLog(html);
    LogMgr.addLog(text);
    LogMgr.addLog(console);
    */

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}