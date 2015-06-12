
#ifndef __ProtocolService__
#define __ProtocolService__

#pragma once

#include <communication/protocols/protocol_renderer.h>

namespace Communication{

    class RenderService{
    public:
        static RenderService& getInstance(){
                    static RenderService    instance;
                    return instance;
                }


    private:
         RenderService(){};
         ~RenderService(){}

         std::shared_ptr<ProtocolRenderer> m_pRenderServer;
    protected:

    };
};

#endif
