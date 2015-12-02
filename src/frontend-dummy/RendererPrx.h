#pragma once
#include <memory>
#include "mocca/net/Endpoint.h"
#include "common/IRenderer.h"

namespace trinity {
    
class RendererPrx : public IRenderer {
    
public:
    
    /// local proxy
    RendererPrx();
    
};
}