#include "DummyRenderer.h"
#include "mocca/log/LogManager.h"

using namespace trinity;

DummyRenderer::DummyRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession)
    : IRenderer(stream, std::move(ioSession)) {}

void DummyRenderer::setIsoValue(const float isoValue) {
    LINFO("(p) iso value set to " + std::to_string(isoValue));
    m_isoValue = isoValue;
}

void DummyRenderer::zoomCamera(float f){
    LINFO("(p) camera zoom: " + std::to_string(f));
}