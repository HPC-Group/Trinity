//
//  IRenderer.h
//  TNG

#ifndef TNG_NETRENDERERCLIENT_h
#define TNG_NETRENDERERCLIENT_h

#include <renderer/IRenderer.h>


//NETWORKING
#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

//COMPRESSION
#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

using namespace mocca;
using namespace mocca::net;


namespace Tuvok{
  namespace Renderer {

    class NetRendererClient : public IRenderer{
    public:
        NetRendererClient(std::string ip,
                          int port,
                          Visibility visibility,
                          Core::Math::Vec2ui resolution,
                          std::string dataset,
                          std::string tf);
        ~NetRendererClient();

		// THREADING
		void startRenderThread(){};
		void pauseRenderThread(){};
		void stopRenderThread(){};

        // BASIC INTERACTION METHODS

        //Camera Controls
        void SetFirstPersonMode(bool mode){};
        void RotateCamera(Core::Math::Vec3f rotation);
        void MoveCamera(Core::Math::Vec3f direction){};
        void SetCamera(Camera c){};
        void SetCameraZoom(float f){};

        //read the last framebuffer
        void ReadFrameBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount){}; //LEGACY
        FrameData ReadFrameBuffer();
        // END BASIC INTERACTION METHODS


		//ISO FUNCTIONS
		virtual void SetIsoValue(float fIsoValue){};
		virtual void SetIsoValueRelative(float fIsovalue){};
		virtual float GetIsoValue() const {};
		virtual void SetIsosurfaceColor(const Core::Math::Vec3f& vColor) {};
		virtual Core::Math::Vec3f GetIsosurfaceColor() const {};
		virtual void SetColorDataset(bool isColor) {};
		//END ISO FUNCTIONS


        virtual void SetClearFrameBuffer(bool bClearFramebuffer){};
        virtual bool GetClearFrameBuffer() const  {};

        virtual void SetViewParameters(float angle, float znear, float zfar) {};
        virtual Core::Math::Mat4f& GetProjectionMatrix() {};
        virtual Core::Math::Mat4f& GetViewMatrix() {};

        virtual bool SetBackgroundColors(Core::Math::Vec3f vTopColor,
                                         Core::Math::Vec3f vBottomColor) {};
        virtual Core::Math::Vec3f GetBackgroundColor(const uint8_t index) {};

        virtual void SetUseLighting(bool bUseLighting) {};
		virtual void SetLightingParameters(Core::Math::Vec4f cAmbient,
			Core::Math::Vec4f cDiffuse,
			Core::Math::Vec4f cSpecular
			) {};
	    virtual const Core::Math::Vec4f GetAmbientColor() {};
	    virtual const Core::Math::Vec4f GetDiffuseColor() {};
	    virtual const Core::Math::Vec4f GetSpecularColor() {};
        virtual void SetSampleRateModifier(float fSampleRateModifier) {};
		virtual void ResetCamera() {};

        virtual Core::Math::Vec2ui GetSize() const {};

        virtual void Resize(const Core::Math::Vec2ui& vWinSize) {};

        virtual void GetVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const {};

        //implement in subclasses
        virtual void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res = false)  {}; //glrenderer

	    virtual void SetLoDFactor(const float f) {};

	    virtual float GetSampleRateModifier() {};

	    virtual void ClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues) {};

	    virtual void SetCompositeMode(ECompositeDisplay displaymode) {};

	    virtual void SetRenderMode(ERenderMode mode) {};

	    virtual void Set1DTransferFunction(std::vector<Core::Math::Vec4f> data) {};

	    virtual Core::Math::Vec4f readVolumePosition(Core::Math::Vec2ui v) {};

	    virtual void setClearViewEnabled(bool b) {};

	    virtual void setClearViewRadius(float f) {};

	    virtual State getRenderState() {};

	    virtual void setRenderState(State renderState) {};

	    virtual void SwitchPagingStrategy(MissingBrickStrategy brickStrategy){};

        //PROPABLY PROTECTED METHODS // HAVE TO CHECK
        virtual void SetDataset(Tuvok::IOPtr dio) {};

        //network methods
        private:
        void connectToServer(std::string ip, int port);
        void initializeRenderer(Visibility visibility,
                                Core::Math::Vec2ui resolution,
                                std::string dataset,
                                std::string tf);

        //member
        private:
        std::unique_ptr<AbstractConnection> connection;
        uint64_t                            _framebuffersize;
        FrameData                           _currentFrame;

    };
  };
};

#endif
