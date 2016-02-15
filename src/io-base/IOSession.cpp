#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"

#include "commands/Vcl.h"
#include "commands/ISerialObjectFactory.h"

#include "DummyIO.h"
#include "IOSession.h"

using namespace trinity::io;
using namespace trinity::commands;



IOSession::IOSession(const std::string& protocol,
                     std::unique_ptr<ICommandFactory> factory,
                     int fileId) :
common::ISession(protocol, std::move(factory))
{
    m_io = createIO(fileId);
}


IOSession::~IOSession() {}



std::unique_ptr<trinity::common::IIO> IOSession::createIO(int fileId, const VclType& type) {
    switch (type) {
        case VclType::DummyIO: {
            
            return std::unique_ptr<DummyIO>(new DummyIO());
            break;
        }
            
        case VclType::FractalIO:
            throw mocca::Error("fractals not supported yet", __FILE__, __LINE__);
            break;
            
        default:
            throw mocca::Error("can't create renderer: no such type", __FILE__, __LINE__);
            break;
    }
}
