#include "io-base/IOSession.h"

#include "common/TrinityError.h"
#include "commands/Vcl.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

IOSession::IOSession(const std::string& protocol, CompressionMode compressionMode, std::unique_ptr<IIO> io)
    : AbstractSession(protocol, compressionMode), m_io(std::move(io)) {
}

std::unique_ptr<ICommandHandler> IOSession::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}
