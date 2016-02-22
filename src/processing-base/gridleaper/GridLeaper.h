#pragma once
#include <memory>

#include "common/IRenderer.h"


namespace trinity {
namespace processing {


class GridLeaper : public common::IRenderer {

public:
    GridLeaper(std::shared_ptr<common::VisStream> stream, std::unique_ptr<common::IIO> ioSession);

    virtual void setIsoValue(const float);

private:
    float m_isoValue;
};
}
}