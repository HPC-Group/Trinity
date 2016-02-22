#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

#include "commands/ISerialObjectFactory.h"
#include "commands/Vcl.h"

#include "DummyIO.h"
#include "IOSession.h"
#include "fractal/FractalIO.h"

using namespace trinity::io;
using namespace trinity::commands;


IOSession::IOSession(const std::string& protocol, std::unique_ptr<ICommandFactory> factory, int fileId)
    : common::ISession(protocol, std::move(factory)) {
    m_io = createIO(fileId);
}


IOSession::~IOSession() {}


std::unique_ptr<trinity::common::IIO> IOSession::createIO(int fileId) {
    switch (fileId) {
    case 0: { // Dummy File

        return std::unique_ptr<DummyIO>(new DummyIO());
        break;
    }

    case 24: // Fractal File
        return std::unique_ptr<FractalIO>(new FractalIO());
        break;

    default:
        throw mocca::Error("can't create renderer: no such type", __FILE__, __LINE__);
        break;
    }
}
