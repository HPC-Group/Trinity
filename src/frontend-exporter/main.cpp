
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"


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
}
