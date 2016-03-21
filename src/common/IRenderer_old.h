//
//  IRenderer.h
//  TNG

#ifndef TNG_IRenderer_h
#define TNG_IRenderer_h


//#include "Service/RenderState.h"
#include <renderer/Context/Context.h>

#include <thread>

namespace Tuvok {
class IOLocal;
typedef std::shared_ptr<IOLocal> IOPtr;

namespace Renderer {

struct FrameData {
    FrameData(uint64_t frame, std::vector<uint8_t> data)
        : _frameID(frame)
        , _data(data){};
    FrameData()
        : _frameID(0)
        , _data(){};

    uint64_t _frameID;
    std::vector<uint8_t> _data; // raw image data
    double _sendTime;
};

class IRenderer {
public:
    IRenderer(){};
    virtual ~IRenderer(){};


    // BASIC INTERACTION METHODS

    // Camera Controls
    virtual void setFirstPersonMode(bool mode) = 0;
    virtual void rotateCamera(Core::Math::Vec3f rotation) = 0;
    virtual void moveCamera(Core::Math::Vec3f direction) = 0;
    // virtual void SetCamera(Camera c) = 0;
    virtual void setCameraZoom(float f) = 0;
    virtual void zoomCamera(float f) = 0;
    virtual void resetCamera() = 0;

    // ISO FUNCTIONS
    virtual void setIsoValue(float fIsoValue) = 0;
    virtual void setIsoValueRelative(float fIsovalue) = 0;
    virtual float getIsoValue() = 0;
    virtual void setIsosurfaceColor(const Core::Math::Vec3f& vColor) = 0;
    virtual Core::Math::Vec3f getIsosurfaceColor() const = 0;
    // END ISO FUNCTIONS


    virtual void setViewParameters(float angle, float znear, float zfar) = 0;
    virtual void setBackgroundColors(std::pair<Core::Math::Vec3f, Core::Math::Vec3f> colors) = 0;
    virtual std::pair<Core::Math::Vec3f, Core::Math::Vec3f> getBackgroundColors(const uint8_t index) = 0;

    virtual void setUseLighting(bool bUseLighting) = 0;
    virtual void setLightingParameters(Core::Math::Vec4f cAmbient, Core::Math::Vec4f cDiffuse, Core::Math::Vec4f cSpecular) = 0;
    virtual const Core::Math::Vec4f getAmbientColor() = 0;
    virtual const Core::Math::Vec4f getDiffuseColor() = 0;
    virtual const Core::Math::Vec4f getSpecularColor() = 0;

    virtual void setSampleRateModifier(float fSampleRateModifier) = 0;
    virtual float getSampleRateModifier() = 0;


    virtual Core::Math::Vec2ui getSize() const = 0;
    virtual void setSize(const Core::Math::Vec2ui& vWinSize) = 0;

    virtual void getVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const = 0;

    // implement in subclasses
    virtual void setViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                             bool decrease_screen_res = false) = 0; // glrenderer

    virtual void SetLoDFactor(const float f) = 0;


    virtual void setClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues) = 0;

    virtual void setCompositeMode(ECompositeDisplay displaymode) = 0;

    virtual void setRenderMode(ERenderMode mode) = 0;

    virtual void set1DTransferFunction(std::vector<Core::Math::Vec4f> data) = 0;

    virtual Core::Math::Vec4f readVolumePosition(Core::Math::Vec2ui v) = 0;

    virtual void setClearViewEnabled(bool b) = 0;

    virtual void setClearViewRadius(float f) = 0;

    // virtual State getRenderState() = 0;

    // virtual void setRenderState(State renderState) = 0;

    virtual void switchPagingStrategy(MissingBrickStrategy brickStrategy) = 0;

