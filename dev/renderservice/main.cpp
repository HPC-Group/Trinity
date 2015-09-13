#include "TuvokService.h"

#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

using namespace ghoul::logging;

int main(int argc, char* argv[]){
    LogManager::initialize(LogManager::LogLevel::Debug, true);

    Log* html = new HTMLLog("RenderService.html");
    Log* text = new TextLog("Rendering.txt");
    Log* console = new ConsoleLog();
    LogMgr.addLog(html);
    LogMgr.addLog(text);
    LogMgr.addLog(console);

    LINFOC("Renderservice", "renderservice running");



    TuvokService service("1234");

    service.run();
}
