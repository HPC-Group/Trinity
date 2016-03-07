#include "processing-base/RenderSession.h"

#include "processing-base/DummyRenderer.h"
#include "gridleaper/GridLeaper.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity::processing;
using namespace trinity::commands;
using namespace trinity::common;


RenderSession::RenderSession(std::unique_ptr<ICommandFactory> factory, const VclType& rendererType, const StreamingParams& params,
                             const std::string& protocol, std::unique_ptr<IOSessionProxy> ioSession)
    : common::ISession(protocol, std::move(factory))
    , m_visPort(m_basePort++)
    , m_visSender(mocca::net::Endpoint(protocol, "localhost", std::to_string(m_visPort)), std::make_shared<common::VisStream>(params)) {
    m_renderer = createRenderer(rendererType, std::move(ioSession));
    m_visSender.startStreaming();
    LINFO("(p) render session started streaming");
}

RenderSession::~RenderSession() {
    m_visSender.endStreaming();
    join();
}


std::unique_ptr<IRenderer> RenderSession::createRenderer(const VclType& rendererType,
                                                         std::unique_ptr<IOSessionProxy> ioSession) {


    switch (rendererType) {
    case VclType::DummyRenderer: {
        return std::unique_ptr<IRenderer>(new DummyRenderer(m_visSender.getStream(), std::move(ioSession)));
        break;
    }

    case VclType::GridLeaper:
        return std::unique_ptr<IRenderer>(new GridLeaper(m_visSender.getStream(), std::move(ioSession)));
        break;

    default:
        throw TrinityError("can't create renderer: no such type", __FILE__, __LINE__);
        break;
    }
}


int RenderSession::getVisPort() const {
    return m_visPort;
}

IRenderer& RenderSession::getRenderer() {
    return *m_renderer;
}
