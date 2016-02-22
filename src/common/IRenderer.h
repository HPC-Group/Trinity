#pragma once
#include <memory>
#include <vector>

#include "IIO.h"
#include "VisStream.h"


namespace trinity {
namespace common {

class IRenderer {

public:
    // all renderers need a vis stream
    IRenderer(std::shared_ptr<VisStream> s)
        : m_visStream(s){};

    // "real" renderers, in addition, need an I/O interface
    IRenderer(std::shared_ptr<VisStream> s, std::unique_ptr<IIO> io)
        : m_visStream(s)
        , m_io(std::move(io)){};

    virtual ~IRenderer(){};

    std::shared_ptr<VisStream> getVisStream() { return m_visStream; };


    // base rendering api begin
    virtual void setIsoValue(const float) = 0;

protected:
    std::shared_ptr<VisStream> m_visStream;
    std::unique_ptr<IIO> m_io;
};
}
}