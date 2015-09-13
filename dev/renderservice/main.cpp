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
*/


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
