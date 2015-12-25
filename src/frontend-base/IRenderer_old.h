//
//  IRenderer.h
//  TNG

#ifndef TNG_IRenderer_h
#define TNG_IRenderer_h


//#include "Service/RenderState.h"
#include <renderer/Context/Context.h>

#include <thread>

namespace Tuvok{
  class IOLocal;
  typedef std::shared_ptr<IOLocal> IOPtr;

  namespace Renderer{

    struct FrameData{
            FrameData(uint64_t frame, std::vector<uint8_t> data):_frameID(frame),_data(data){};
            FrameData():_frameID(0),_data(){};

            uint64_t                _frameID;
            std::vector<uint8_t>    _data; //raw image data
			double					_sendTime;
    };

    class IRenderer{
    public:
        IRenderer(){};
        virtual ~IRenderer(){};

		// THREADING
		virtual void startRenderThread() = 0;
		virtual void pauseRenderThread() = 0;
		virtual void stopRenderThread() = 0;

        // BASIC INTERACTION METHODS

        //Camera Controls
        virtual void SetFirstPersonMode(bool mode)  = 0;
        virtual void RotateCamera(Core::Math::Vec3f rotation) = 0;
        virtual void MoveCamera(Core::Math::Vec3f direction) = 0;
        //virtual void SetCamera(Camera c) = 0;
        virtual void SetCameraZoom(float f) = 0;
        virtual void ZoomCamera(float f) = 0;

        //read the last framebuffer
        virtual void ReadFrameBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount) = 0; //LEGACY
        virtual FrameData& ReadFrameBuffer() = 0;
        // END BASIC INTERACTION METHODS


		//ISO FUNCTIONS
		virtual void SetIsoValue(float fIsoValue) = 0;
		virtual void SetIsoValueRelative(float fIsovalue) = 0;
		virtual float GetIsoValue() = 0;
		virtual void SetIsosurfaceColor(const Core::Math::Vec3f& vColor) = 0;
		virtual Core::Math::Vec3f GetIsosurfaceColor() const = 0;
		virtual void SetColorDataset(bool isColor) = 0;
		//END ISO FUNCTIONS


        virtual void SetClearFrameBuffer(bool bClearFramebuffer) = 0;
        virtual bool GetClearFrameBuffer() const  = 0;

        virtual void SetViewParameters(float angle, float znear, float zfar) = 0;
        virtual Core::Math::Mat4f& GetProjectionMatrix() = 0;
        virtual Core::Math::Mat4f& GetViewMatrix() = 0;

        virtual bool SetBackgroundColors(Core::Math::Vec3f vTopColor,
                                         Core::Math::Vec3f vBottomColor) = 0;
        virtual Core::Math::Vec3f GetBackgroundColor(const uint8_t index) = 0;

        virtual void SetUseLighting(bool bUseLighting) = 0;
		virtual void SetLightingParameters(Core::Math::Vec4f cAmbient,
			Core::Math::Vec4f cDiffuse,
			Core::Math::Vec4f cSpecular
			) = 0;
	    virtual const Core::Math::Vec4f GetAmbientColor() = 0;
	    virtual const Core::Math::Vec4f GetDiffuseColor() = 0;
	    virtual const Core::Math::Vec4f GetSpecularColor() = 0;
        virtual void SetSampleRateModifier(float fSampleRateModifier) = 0;
		virtual void ResetCamera() = 0;

        virtual Core::Math::Vec2ui GetSize() const = 0;

        virtual void Resize(const Core::Math::Vec2ui& vWinSize) = 0;

        virtual void GetVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const = 0;

        //implement in subclasses
        virtual void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res = false)  = 0; //glrenderer

	    virtual void SetLoDFactor(const float f) = 0;

	    virtual float GetSampleRateModifier() = 0;

	    virtual void ClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues) = 0;

	    virtual void SetCompositeMode(ECompositeDisplay displaymode) = 0;

	    virtual void SetRenderMode(ERenderMode mode) = 0;

	    virtual void Set1DTransferFunction(std::vector<Core::Math::Vec4f> data) = 0;

	    virtual Core::Math::Vec4f readVolumePosition(Core::Math::Vec2ui v) = 0;

	    virtual void setClearViewEnabled(bool b) = 0;

	    virtual void setClearViewRadius(float f) = 0;

	    //virtual State getRenderState() = 0;

	    //virtual void setRenderState(State renderState) = 0;

	    virtual void SwitchPagingStrategy(MissingBrickStrategy brickStrategy) = 0;

        //PROPABLY PROTECTED METHODS // HAVE TO CHECK
        virtual void SetDataset(Tuvok::IOPtr dio) = 0;

        virtual void run() = 0;

        virtual void stopRenderer() = 0;
    };
  };
};

#endif
