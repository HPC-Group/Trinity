#ifndef __RenderService__
#define __RenderService__

#pragma once


#include <unordered_map>
#include <mutex>
#include <atomic>
#include <string>
#include <memory>
#include <thread>

#include "../AbstrRenderer.h"
#include "../Context/Context.h"

#include <core/Time/Timer.h>

#include "../RenderEnums.h"
#include "RenderStatev2.h"
#include "RenderCmd.h"

#include <queue>
#include <core/CmdQueue.h>
namespace Tuvok{
    namespace Renderer{
        namespace Service{

            class ServiceEntry{
            public:
                ServiceEntry(uint16_t identifier, std::shared_ptr<AbstrRenderer> renderer, std::shared_ptr<Context::Context> context = nullptr):
                m_uiIdentifier(identifier), m_pRenderer(renderer), m_pContext(context),m_pRenderState(){
					m_bCanceled = new bool();
					*m_bCanceled = true;
                    m_pRenderState = std::make_shared<State>();
                    m_pCommandList = std::make_shared<Core::CmdQueue>();
                }

                std::shared_ptr<AbstrRenderer> getRenderer() const{
                    return m_pRenderer;
                }
                std::shared_ptr<Context::Context> getContext() const{
                    return m_pContext;
                }
                std::shared_ptr<State> getRenderState() const{
                    return m_pRenderState;
                }
                uint16_t getIdentifier() const{
                    return m_uiIdentifier;
                }

                void setCanceled(bool b) { *m_bCanceled = b;}

                bool getIsCanceled() const {
                    return *m_bCanceled;
                }

                void addCommand(std::shared_ptr<Core::AbstrCmd> c){
                    m_pCommandList->push(c);
                }

                void setRenderState(State s){
                    m_pRenderState.reset();
                    m_pRenderState = std::make_shared<State>(s);
                }

                void handleCommandQueue();
            private:
                uint16_t                            m_uiIdentifier;
                std::shared_ptr<Context::Context>   m_pContext;
                std::shared_ptr<AbstrRenderer>      m_pRenderer;
                std::shared_ptr<State>              m_pRenderState;

                std::shared_ptr<Core::CmdQueue>     m_pCommandList;

                bool*                               m_bCanceled;
            };


            class RenderServer{
            public:
                static RenderServer& getInstance(){
                    static RenderServer    instance;
                    return instance;
                }

                uint64_t createNewRenderer(Visibility visibility, Core::Math::Vec2ui resolution,std::string dataset,std::string tf, std::string ioHost = "localhost", uint16_t ioPort = 55555);



                std::shared_ptr<Context::Context> getContextPtr(uint16_t handle);
                std::shared_ptr<AbstrRenderer> getRendererPtr(uint16_t handle);

                void killAllThreads();

            private:
                std::shared_ptr<Context::Context> initNewContext(Visibility visibility, Core::Math::Vec2ui resolution);
                std::shared_ptr<AbstrRenderer> initNewRenderer(std::shared_ptr<Context::Context> context, Core::Math::Vec2ui resolution,std::string dataset,std::string tf, std::string ioHost, uint16_t ioPort);

                void singleRenderThreadLoop(uint16_t serviceHandle, Visibility visibility, Core::Math::Vec2ui resolution,std::string dataset,std::string tf, std::string ioHost, uint16_t ioPort);


                RenderServer(){
                    #if defined __APPLE__
                    m_eServerSystem = System::OSX;
                    #elif defined _WIN32
                    m_eServerSystem = System::Windows;
                    #else
                    m_eServerSystem = System::Linux;
                    #endif
                };
                ~RenderServer(){}

                std::vector<std::shared_ptr<std::thread>>   m_runThreads;

                RenderServer*                               m_rsInstance;
                std::atomic<uint16_t>						m_servieIdentifierSource;
                std::recursive_mutex						m_rsMutex;
                std::unordered_map<uint16_t, ServiceEntry>  m_ServiceMap;
                System                                      m_eServerSystem;

            //interactions
            public:
                uint8_t checkIfRenderThreadIsRunning(uint16_t handle);
                void stopRenderThread(uint16_t handle);

                void rotateCamera(uint16_t handle, Core::Math::Vec3f rotate);
                void moveCamera(uint16_t handle, Core::Math::Vec3f direction);
                void toogleFirstPerson(uint16_t handle, bool toggle);
                void zoomCamera(uint16_t handle, float zoom);

                void setRenderMode(uint16_t handle, Tuvok::Renderer::ERenderMode mode);
                void setCompositeMode(uint16_t handle, Tuvok::Renderer::ECompositeDisplay mode);

                //clearview
                void setClearViewEnabled(uint16_t handle,bool b);
				void clearViewPicking(uint16_t handle, Core::Math::Vec2ui pixelposition);
				void setClearViewRadius(uint16_t handle, float f);
            };
        }
    };
};

#endif /* defined(__RenderServer__) */
