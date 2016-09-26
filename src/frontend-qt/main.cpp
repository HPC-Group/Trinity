#include "MainWindow.h"
#include <QApplication>
#include <QFile>

#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"

#include <opengl-base/OpenGlHeadlessContext.h>

using namespace mocca::net;

void init() {
  using mocca::LogManager;
  LogManager::initialize(LogManager::LogLevel::Debug, true);
  auto console = new mocca::ConsoleLog();
  LogMgr.addLog(console);
  ConnectionFactorySelector::addDefaultFactories();
}

int main(int argc, char *argv[])
{
    init();
    QApplication a(argc, argv);


    /*QFile File("stylesheet.css");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    a.setStyleSheet(StyleSheet);*/

    MainWindow w;
    w.show();
    return a.exec();
}
