#include <renderer/RenderManager.h>
#include <renderer/IRenderer.h>
#include "CommandReciever.h"

#include <core/Math/Vectors.h>
#include <core/Time/Timer.h>

#include <memory>


#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>
#include <net/Error.h>

#include <thread>

#include <renderer/NetworkRenderer/NetCommands.h>


#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"


using namespace Tuvok::Renderer;
using namespace Core::Math;
using namespace mocca;
using namespace mocca::net;
using namespace Core::Time;
using namespace ghoul::logging;


int main(int argc, char* argv[]){
    if(argc != 6) return -1;

    LogManager::initialize(LogManager::LogLevel::Info, true);

    std::string htmlfile = "RenderProcess_"+std::string(argv[1])+".html";

    Log* html = new HTMLLog(htmlfile);
    Log* text = new TextLog("Rendering.txt");
    Log* console = new ConsoleLog();
    LogMgr.addLog(html);
    LogMgr.addLog(text);
    LogMgr.addLog(console);

    LINFOC("Renderprocess", "started new renderprocess");
    LINFOC("Renderprocess", "listening on port "<< argv[1]);


    std::shared_ptr<IRenderer> renderer = RenderManager::getInstance().createRenderer(
                                                            Visibility::Windowed,                           //displaymode
                                                            Vec2ui(std::atoi(argv[4]),std::atoi(argv[5])),  //resolution
                                                            argv[2],                                        //dataset
                                                            argv[3],                                        // tf
                                                            false);                                         //start threaded


    CommandReciever cmdrec(std::atoi(argv[1]),renderer);
    cmdrec.startThread();

    renderer->run();

    return 1;
}
