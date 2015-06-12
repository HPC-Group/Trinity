
#ifndef __Protocol_Renderer__
#define __Protocol_Renderer__

#pragma once

#include <core/Math/Vectors.h>
#include <renderer/RenderEnums.h>

namespace Communication{

    class ProtocolRenderer{

        public:
                virtual uint64_t createNewRenderer( Tuvok::Renderer::Visibility visibility,
                                                    Core::Math::Vec2ui          resolution,
                                                    std::string                 dataset,
                                                    std::string                 tf,
                                                    std::string                 ioHost = "localhost",
                                                    uint16_t                    ioPort = 44322) = 0;


                virtual uint8_t checkIfRenderThreadIsRunning(uint16_t handle) = 0;
                virtual void stopRenderThread(uint16_t handle) = 0;

                virtual void rotateCamera(uint16_t handle, Core::Math::Vec3f rotate) = 0;
                virtual void moveCamera(uint16_t handle, Core::Math::Vec3f direction) = 0;
                virtual void toogleFirstPerson(uint16_t handle, bool toggle) = 0;
                virtual void zoomCamera(uint16_t handle, float zoom) = 0;

                virtual void setRenderMode(uint16_t handle, Tuvok::Renderer::ERenderMode mode) = 0;
                virtual void setCompositeMode(uint16_t handle, Tuvok::Renderer::ECompositeDisplay mode) = 0;

                //clearview
                virtual void setClearViewEnabled(uint16_t handle,bool b) = 0;
				virtual void clearViewPicking(uint16_t handle, Core::Math::Vec2ui pixelposition) = 0;
				virtual void setClearViewRadius(uint16_t handle, float f) = 0;

    };
}

#endif
