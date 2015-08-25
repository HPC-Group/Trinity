#ifndef TUVOK_CONTEXT
#define TUVOK_CONTEXT

#include "logging/logmanager.h"
#include "../RenderEnums.h"

#include <core/Math/Vectors.h>

#include <vector>
#include <unordered_map>

#include "ContextMutex.h"


namespace Tuvok{
    namespace Renderer{
        namespace Context{

        class ContextFrames{
        public:
            ContextFrames(){};
            ~ContextFrames(){};

            void recordFrame(){};

            std::vector<uint8_t> getNextFrame()const {
                return m_frames.begin()->second;
            };
            std::vector<uint8_t> getLatestFrame()const{
                return m_frames.end()->second;
            }

            Core::Math::Vec2ui   getResolution() const {return m_vResolution;}
            uint8_t              getComponentCount() const {return m_iComponentCount;}
            uint8_t              getBytePerComponent() const {return m_iBytePerComponent;}

        private:
            Core::Math::Vec2ui                                  m_vResolution;
            uint8_t                                             m_iComponentCount;
            uint8_t                                             m_iBytePerComponent;
            std::unordered_map<uint16_t, std::vector<uint8_t>>  m_frames;

            uint16_t                                            m_latestRequestedFrame;
        };

        class Context{
        public:
            virtual void lockContext() = 0;
            virtual void unlockContext() = 0;
            virtual bool initContext() = 0;
            virtual bool deleteContext() = 0;

            virtual void* getContextItem() = 0;

            virtual void frameFinished() = 0;

            virtual int storeFinalFrameToTNG(std::string name) = 0;

            virtual void ReadBackBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount) = 0;

            Core::Math::Vec2ui getContextResolution() const{ return m_vResolution;}
        protected:
            Context(std::string windowname, System system, Visibility visibility,Core::Math::Vec2ui resolution):
                m_sName(windowname),
                m_vResolution(resolution),
                m_eVisibility(visibility),
                m_eSystem(system)
            {};
            ~Context(){};

            std::string             m_sName;
            Core::Math::Vec2ui      m_vResolution;
            System                  m_eSystem;
            Visibility              m_eVisibility;
        };

        };
    };
};

#endif
