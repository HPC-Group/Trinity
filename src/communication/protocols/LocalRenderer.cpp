#include "LocalRenderer.h"

using namespace Communication;
using namespace Tuvok::Renderer::Service;

LocalRenderer::LocalRenderer()
{

    //ctor
}

LocalRenderer::~LocalRenderer()
{
    //dtor
}


uint64_t LocalRenderer::createNewRenderer( Tuvok::Renderer::Visibility visibility,
                                                    Core::Math::Vec2ui          resolution,
                                                    std::string                 dataset,
                                                    std::string                 tf,
                                                    std::string                 ioHost,
                                                    uint16_t                    ioPort){
    m_iRenderHandle = RenderServer::getInstance().createNewRenderer(visibility, resolution,dataset,tf,ioHost,ioPort);
}

uint8_t LocalRenderer::checkIfRenderThreadIsRunning(){
    RenderServer::getInstance().checkIfRenderThreadIsRunning(m_iRenderHandle);
}
void LocalRenderer::stopRenderThread(){
    RenderServer::getInstance().stopRenderThread(m_iRenderHandle);
}

void LocalRenderer::rotateCamera(Core::Math::Vec3f rotate){
    RenderServer::getInstance().rotateCamera(m_iRenderHandle, rotate);
}
void LocalRenderer::moveCamera(Core::Math::Vec3f direction){
    RenderServer::getInstance().moveCamera(m_iRenderHandle, direction);
}
void LocalRenderer::toggleFirstPerson(bool toggle){
    RenderServer::getInstance().toogleFirstPerson(m_iRenderHandle,toggle);
}
void LocalRenderer::zoomCamera(float zoom){
    RenderServer::getInstance().zoomCamera(m_iRenderHandle,zoom);
}

void LocalRenderer::setRenderMode(Tuvok::Renderer::ERenderMode mode){
    RenderServer::getInstance().setRenderMode(m_iRenderHandle,mode);
}
void LocalRenderer::setCompositeMode(Tuvok::Renderer::ECompositeDisplay mode){
    RenderServer::getInstance().setCompositeMode(m_iRenderHandle,mode);
}

//clearview
void LocalRenderer::setClearViewEnabled(bool b){
    RenderServer::getInstance().setClearViewEnabled(m_iRenderHandle,b);
}
void LocalRenderer::clearViewPicking(Core::Math::Vec2ui pixelposition){
    RenderServer::getInstance().clearViewPicking(m_iRenderHandle,pixelposition);
}
void LocalRenderer::setClearViewRadius(float f){
    RenderServer::getInstance().setClearViewRadius(m_iRenderHandle,f);
}
