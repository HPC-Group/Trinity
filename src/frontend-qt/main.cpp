#include "window.h"
#include <QApplication>
#include <QSurfaceFormat>

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

int main(int argc, char* argv[]) {
  init();
  
  /*
  QSurfaceFormat format;
  format.setDepthBufferSize(24); //if you need it
  format.setStencilBufferSize(8); //if you need it
  format.setVersion(3, 2); // OpenGL version
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
*/
  
  QApplication a(argc, argv);
  Window w;
  w.show();
  
  return a.exec();
}
