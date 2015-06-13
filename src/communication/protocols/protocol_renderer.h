
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


                virtual uint8_t checkIfRenderThreadIsRunning() = 0;
                virtual void stopRenderThread() = 0;

                virtual void rotateCamera(Core::Math::Vec3f rotate) = 0;
                virtual void moveCamera(Core::Math::Vec3f direction) = 0;
                virtual void toggleFirstPerson(bool toggle) = 0;
                virtual void zoomCamera(float zoom) = 0;

                virtual void setRenderMode(Tuvok::Renderer::ERenderMode mode) = 0;
                virtual void setCompositeMode(Tuvok::Renderer::ECompositeDisplay mode) = 0;

                //clearview
                virtual void setClearViewEnabled(bool b) = 0;
				virtual void clearViewPicking(Core::Math::Vec2ui pixelposition) = 0;
				virtual void setClearViewRadius(float f) = 0;

				uint16_t getRenderHandle() const {return m_iRenderHandle;}
        protected:
                uint16_t                                    m_iRenderHandle;

    };
}

#endif
