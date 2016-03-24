#pragma once
#include <memory>

#include "common/IRenderer.h"


namespace trinity {
class DummyRenderer : public IRenderer {
public:
    DummyRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);

    void setRenderMode(ERenderMode mode) {}
    bool supportsRenderMode(ERenderMode mode) { return true; }

    void setIsoValue(float) override;
    void initContext() override {};
	void deleteContext() override {};

    // unit test purposes
    float getIsoValue() const { return m_isoValue; };
    void zoomCamera(float f) override;

private:
    float m_isoValue;
};
}
