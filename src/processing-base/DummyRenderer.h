#pragma once
#include <memory>

#include "common/IRenderer.h"


namespace trinity {
class DummyRenderer : public IRenderer {
public:
    DummyRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);

    void setIsoValue(const float) override;
    void initContext() override {};
	void deleteContext() override {};

    // unit test purposes
    float getIsoValue() const { return m_isoValue; };
    void zoomCamera(float f) override;

private:
    float m_isoValue;
};
}
