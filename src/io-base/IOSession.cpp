#include "io-base/IOSession.h"

#include "common/TrinityError.h"
#include "commands/Vcl.h"
#include "fractal/FractalIO.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

IOSession::IOSession(const std::string& protocol, std::unique_ptr<IIO> io)
    : AbstractSession(protocol), m_io(std::move(io)) {
}

std::unique_ptr<trinity::IIO> IOSession::createIO(const std::string& fileId, IListData& listData) {
    return std::unique_ptr<FractalIO>(new FractalIO(fileId, listData));
}

std::unique_ptr<ICommandHandler> IOSession::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}
