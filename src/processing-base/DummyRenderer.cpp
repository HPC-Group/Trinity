#include "DummyRenderer.h"
#include "mocca/log/LogManager.h"

using namespace trinity::processing;
using namespace trinity::common;

DummyRenderer::DummyRenderer(std::shared_ptr<VisStream> stream,
                             std::unique_ptr<IIO> ioSession) :
IRenderer(stream, std::move(ioSession)) {
}

void DummyRenderer::setIsoValue(const float isoValue) {
    LINFO("(p) iso value set to " + std::to_string(isoValue));
    m_isoValue = isoValue;
    LINFO("(p) lod count: " + std::to_string(m_io->getLODLevelCount()));
}