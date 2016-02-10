#include "DummyRenderer.h"
#include "mocca/log/LogManager.h"

using namespace trinity::processing;
using namespace trinity::common;

DummyRenderer::DummyRenderer(std::shared_ptr<VisStream> stream) :
IRenderer(stream) {
}

void DummyRenderer::setIsoValue(const float isoValue) {
    LINFO("(p) iso value set to " + std::to_string(isoValue));
    m_isoValue = isoValue;
}