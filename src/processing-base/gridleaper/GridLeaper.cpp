#include "GridLeaper.h"
#include "mocca/log/LogManager.h"

using namespace trinity::processing;
using namespace trinity::common;

GridLeaper::GridLeaper(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession)
    : IRenderer(stream, std::move(ioSession)) {
    LINFO("(p) grid leaper created");
}

void GridLeaper::setIsoValue(const float isoValue) {
    LINFO("(p) iso value of gridleaper set to " + std::to_string(isoValue));
    m_isoValue = isoValue;
    // that is how you call io functios
    // LINFO("(p) lod count: " + std::to_string(m_io->getLODLevelCount()));
}