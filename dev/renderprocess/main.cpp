/*
Einfach Einleitung:

Renderservice bindet einen Port zu dem sich frontends verbinden.
Ein frontend erh�lt darauf hin einen neuen Port unter welchem
der eigentliche Rendererprocess zu erreichen ist. Sobald ein frontend
diesen Port erhalten hat verbindet er sich zu diesem und gibt die
Verbindung zum renderservice auf.

Um diese App zu benutzen muss sowohl der renderservice, als auch
renderprocess im release gebaut werden. Beide binarys m�ssen sich im
gleichen Ordner befinden (sp�ter k�nnte man dies noch anders gestalten)
weiterhin m�ssen sich in dem gleichen Ordner die Shader und Datens�tze
befinden, so wie bisher auch.

Danach muss nur der Renderservice gestartet werden. Sobald sich ein
frontend verbindet startet dieser von selbst einen Renderprocess mit
dem angefragtem Datensatz.

F�r die Zukunft sollten wir den renderservice und renderprocess per
default im gleichen Ordner bauen, aktuell m�ssen diese noch verschoben
werden. Am einfachsten sit es den service zu bauen und die binary dann
in den release ordner vom process zu kopieren.

Der QtNetwork_Client funktioniert mit diesem Ansatz und wurde unter
Linux/Windows/osX getestet. Wenn du weiterhin den rendermanager benutzt
um ein renderer zu erstellen �ndert sich nichts da dieser "portshift"
komplett vor dir versteckt wird.

//UPDATE
PROJECTE WERDEN NUN IN 
	trinity/build/bin/Release
	trinity/build/bin/Debug
gebaut, kein copy mehr n�tig
*/


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
                                                            Visibility::hidden,                           //displaymode
                                                            Vec2ui(std::atoi(argv[4]),std::atoi(argv[5])),  //resolution
                                                            argv[2],                                        //dataset
                                                            argv[3],                                        // tf
                                                            false);                                         //start threaded


    CommandReciever cmdrec(std::atoi(argv[1]),renderer);
    cmdrec.startThread();

    renderer->run();


	cmdrec.joinThread();
    return 1;
}
