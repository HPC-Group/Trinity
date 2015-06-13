#ifndef LOCALRENDERER_H
#define LOCALRENDERER_H

#pragma once
#include "protocol_renderer.h"
#include <renderer/Service/RenderServer.h>

namespace Communication{
class LocalRenderer : public ProtocolRenderer
{
    public:
        LocalRenderer();
        virtual ~LocalRenderer();


        uint64_t createNewRenderer( Tuvok::Renderer::Visibility visibility,
                                                    Core::Math::Vec2ui          resolution,
                                                    std::string                 dataset,
                                                    std::string                 tf,
                                                    std::string                 ioHost = "localhost",
                                                    uint16_t                    ioPort = 44322);


         uint8_t checkIfRenderThreadIsRunning();
         void stopRenderThread();

         void rotateCamera(Core::Math::Vec3f rotate);
         void moveCamera(Core::Math::Vec3f direction);
         void toggleFirstPerson(bool toggle);
         void zoomCamera(float zoom);

         void setRenderMode(Tuvok::Renderer::ERenderMode mode);
         void setCompositeMode(Tuvok::Renderer::ECompositeDisplay mode);

         //clearview
         void setClearViewEnabled(bool b);
		 void clearViewPicking(Core::Math::Vec2ui pixelposition);
		 void setClearViewRadius(float f);

    protected:

    private:

};
};
#endif // LOCALRENDERER_H
