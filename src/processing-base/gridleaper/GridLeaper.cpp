#include "GridLeaper.h"
#include "mocca/log/LogManager.h"

#include <opengl-base/OpenGlHeadlessContext.h>

using namespace trinity::processing;
using namespace trinity::common;

GridLeaper::GridLeaper(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession)
    : IRenderer(stream, std::move(ioSession)) {
    OpenGlHeadlessContext context;

    if (!context.isValid()) {
        LERROR("can't create context");
      std::cout << "can't create context" << std::endl;
    }

    LINFO("(p) grid leaper created. OpenGL Version " << context.getVersion());
}

void GridLeaper::setIsoValue(const float isoValue) {
    LINFO("(p) iso value of gridleaper set to " + std::to_string(isoValue));
    m_isoValue = isoValue;
    // that is how you call io functios
    //LINFO("(p) lod count: " + std::to_string(m_io->getLODLevelCount()));
}
