#pragma once
#include <memory>

#include "processing-base/AbstractRenderer.h"

namespace trinity {
class DummyRenderer : public AbstractRenderer {
public:
    DummyRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);

    void initContext() override{};
    void deleteContext() override{};
    void paintInternal(PaintLevel paintlevel) override {}

    bool isIdle() override { return false; }
    bool proceedRendering() override { return true; }

private:
    float m_isoValue;
};
}
