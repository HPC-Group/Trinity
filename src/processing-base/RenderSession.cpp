#include "processing-base/RenderSession.h"

#include "common/TrinityError.h"
#include "processing-base/DummyRenderer.h"
#include "simplerenderer/SimpleRenderer.h"
#include "gridleaper/GridLeaper.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

RenderSession::RenderSession(const VclType& rendererType, const StreamingParams& params,
                             const std::string& protocol, std::unique_ptr<IOSessionProxy> ioSession)
    : AbstractSession(protocol)
    // FIXME dmc: "localhost" should be "*", but then the tests fail -> find out why!
    , m_visSender(mocca::net::Endpoint(protocol, "localhost", mocca::net::Endpoint::autoPort()), std::make_shared<VisStream>(params)) {
    m_renderer = createRenderer(rendererType, std::move(ioSession));
    m_visSender.startStreaming();
    LINFO("(p) render session started streaming");
}

RenderSession::~RenderSession() {
	LINFO("(p) trying to end the vis stream");
    m_visSender.endStreaming();
	LINFO("(p) vis stream closed");
    join();
	LINFO("(p) render session joined");
}

std::unique_ptr<IRenderer> RenderSession::createRenderer(const VclType& rendererType, std::unique_ptr<IOSessionProxy> ioSession) {
    switch (rendererType) {
    case VclType::DummyRenderer:
        return std::unique_ptr<IRenderer>(new DummyRenderer(m_visSender.getStream(), std::move(ioSession)));
        break;
    case VclType::SimpleRenderer:
        return std::unique_ptr<IRenderer>(new SimpleRenderer(m_visSender.getStream(), std::move(ioSession)));
        break;
    case VclType::GridLeapingRenderer:
        return std::unique_ptr<IRenderer>(new GridLeaper(m_visSender.getStream(), std::move(ioSession)));
        break;
    default:
        throw TrinityError("can't create renderer: no such type", __FILE__, __LINE__);
        break;
    }
}

std::string RenderSession::getVisPort() const {
    return m_visSender.getPort();
}

IRenderer& RenderSession::getRenderer() {
    return *m_renderer;
}

std::unique_ptr<ICommandHandler> RenderSession::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}

void RenderSession::performThreadSpecificTeardown() {
	std::thread::id threadId = std::this_thread::get_id();
	LINFO("rendersession performs specific rendering teardown from thread " << threadId);
	m_renderer->deleteContext();
}
