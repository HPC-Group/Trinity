#include "connectionSinglton.h"

using namespace std;
using namespace mocca::net;

static int reconnectInSec = 5;

ConnectionSingleton::ConnectionSingleton()
    : _ioNode(nullptr)
    , _processingNode(nullptr)
    , m_renderer(nullptr)
    , m_initDone(false)
    , m_rendererType(trinity::VclType::SimpleRenderer) {}

ConnectionSingleton::~ConnectionSingleton() {
    _ioNode = nullptr;
    _processingNode = nullptr;
    m_renderer = nullptr;
}

trinity::VclType ConnectionSingleton::getRendererType() const {
    return m_rendererType;
}

void ConnectionSingleton::setRendererType(const trinity::VclType& renderer) {
    m_rendererType = renderer;
}

trinity::IONodeProxy* ConnectionSingleton::ioNode() {
    return _ioNode.get();
}

trinity::ProcessingNodeProxy& ConnectionSingleton::processingNode() {
    return *_processingNode;
}
trinity::RendererProxy& ConnectionSingleton::renderer() {
    return *m_renderer;
}

void ConnectionSingleton::connectIO(const std::string& hostname, const std::string& port) {
    std::cout << hostname << " " << port << std::endl;
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), hostname, port);
    _ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
}

void ConnectionSingleton::connectProcessing(const std::string& hostname, const std::string& port) {
    std::cout << hostname << " " << port << std::endl;
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), hostname, port);

    _processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
}

void ConnectionSingleton::initRenderer(const std::string& iohostname, const std::string& ioport, const uint32_t width,
                                       const uint32_t height, const std::string fileId) {

    trinity::StreamingParams params(width, height);

    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), iohostname, ioport);

    // the file id will be available after implementing the listdata command
    try {
        m_renderer = _processingNode->initRenderer(m_rendererType, fileId, endpointIO, params);
        //_renderer->initContext();
    } catch (const trinity::TrinityError&) {
        LERROR("(qt) no connection to a renderer");
    }

    m_renderer->startRendering();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    m_initDone = true;
}

void ConnectionSingleton::reset() {
    m_renderer = nullptr;
    m_initDone = false;
}