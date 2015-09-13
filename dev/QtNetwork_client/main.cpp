/*
Einfach Einleitung:

Renderservice bindet einen Port zu dem sich frontends verbinden.
Ein frontend erhält darauf hin einen neuen Port unter welchem
der eigentliche Rendererprocess zu erreichen ist. Sobald ein frontend
diesen Port erhalten hat verbindet er sich zu diesem und gibt die
Verbindung zum renderservice auf.

Um diese App zu benutzen muss sowohl der renderservice, als auch
renderprocess im release gebaut werden. Beide binarys müssen sich im
gleichen Ordner befinden (später könnte man dies noch anders gestalten)
weiterhin müssen sich in dem gleichen Ordner die Shader und Datensätze
befinden, so wie bisher auch.

Danach muss nur der Renderservice gestartet werden. Sobald sich ein
frontend verbindet startet dieser von selbst einen Renderprocess mit
dem angefragtem Datensatz.

Für die Zukunft sollten wir den renderservice und renderprocess per
default im gleichen Ordner bauen, aktuell müssen diese noch verschoben
werden. Am einfachsten sit es den service zu bauen und die binary dann
in den release ordner vom process zu kopieren.

Der QtNetwork_Client funktioniert mit diesem Ansatz und wurde unter
Linux/Windows/osX getestet. Wenn du weiterhin den rendermanager benutzt
um ein renderer zu erstellen ändert sich nichts da dieser "portshift"
komplett vor dir versteckt wird.

//UPDATE
PROJECTE WERDEN NUN IN
trinity/build/bin/Release
trinity/build/bin/Debug
gebaut, kein copy mehr nötig
*/


#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(2, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    client w;
    w.show();

    return a.exec();
}
