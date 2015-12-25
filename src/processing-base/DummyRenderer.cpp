#include "DummyRenderer.h"

using namespace trinity::processing;
using namespace trinity::common;

DummyRenderer::DummyRenderer(std::shared_ptr<common::VisStream> stream) :
IRenderer(stream) {
}