#include "DummyRenderer.h"
#include "mocca/log/LogManager.h"

using namespace trinity;

DummyRenderer::DummyRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession)
    : AbstractRenderer(stream, std::move(ioSession)) {}