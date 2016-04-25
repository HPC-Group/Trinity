#include "Connections.h"

using namespace std;
using namespace mocca::net;

static int reconnectInSec = 5;

Connections::Connections()
    : m_ioNode(nullptr)
    , m_processingNode(nullptr)
    , m_renderer(nullptr)
    , m_initDone(false)
    , m_rendererType(trinity::VclType::GridLeapingRenderer) {}

trinity::VclType Connections::getRendererType() const {
    return m_rendererType;
}

void Connections::setRendererType(const trinity::VclType& renderer) {
    m_rendererType = renderer;
}

trinity::IONodeProxy* Connections::ioNode() {
    return m_ioNode.get();
}

trinity::ProcessingNodeProxy& Connections::processingNode() {
    return *m_processingNode;
}
trinity::RendererProxy& Connections::renderer() {
    return *m_renderer;
}

void Connections::connectIO(const std::string& hostname, const std::string& port) {
    std::cout << hostname << " " << port << std::endl;
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), hostname, port);
    m_ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
}

void Connections::connectProcessing(const std::string& hostname, const std::string& port) {
    std::cout << hostname << " " << port << std::endl;
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), hostname, port);

    m_processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
}

void Connections::initRenderer(const std::string& iohostname, const std::string& ioport, const uint32_t width,
                                       const uint32_t height, const std::string fileId) {

    trinity::StreamingParams params(width, height);

    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), iohostname, ioport);
    m_renderer = m_processingNode->initRenderer(m_rendererType, fileId, endpointIO, params);
    m_renderer->startRendering();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    m_initDone = true;
}

void Connections::reset() {
    m_renderer = nullptr;
    m_initDone = false;
}