#pragma once
#include <memory>

#include "common/IRenderer.h"


namespace trinity {
class DummyRenderer : public IRenderer {
public:
    DummyRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);

    virtual void setIsoValue(const float);
    virtual void initContext(){};

    // unit test purposes
    float getIsoValue() const { return m_isoValue; };
    int getLodLevelCountFromIO() { return m_io->getLODLevelCount(); }

private:
    float m_isoValue;
};
}
