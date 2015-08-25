#ifndef TNG_AbstrRenderer_h
#define TNG_AbstrRenderer_h

#include "IRenderer.h"
#include "CullingLOD.h"

namespace Tuvok{
  class RenderRegion;
  typedef std::shared_ptr<RenderRegion> RenderRegionPtr;

  class ExtendedPlane;
  typedef std::shared_ptr<ExtendedPlane> ExtendedPlanePtr;

  namespace Renderer{
      class Camera;
      typedef std::shared_ptr<Camera> CameraPtr;

      class AbstrRenderer : public IRenderer{
      public:
        AbstrRenderer(                    std::shared_ptr<Tuvok::Renderer::Context::Context> context,
                                            Core::Math::Vec2ui vWinSize = Core::Math::Vec2ui(640, 480),
                                            ERenderMode mode = RM_ISOSURFACE);
        virtual ~AbstrRenderer();

        virtual bool Initialize();
        virtual void Cleanup();
        virtual bool Paint();

        virtual void SetClearFrameBuffer(bool bClearFramebuffer);
        virtual bool GetClearFrameBuffer() const;

        virtual void SetViewParameters(float angle, float znear, float zfar);

        virtual Core::Math::Mat4f& GetProjectionMatrix();
        virtual Core::Math::Mat4f& GetViewMatrix();

        virtual bool SetBackgroundColors(Core::Math::Vec3f vTopColor,
                                         Core::Math::Vec3f vBottomColor);
        virtual Core::Math::Vec3f GetBackgroundColor(const uint8_t index);

        virtual void SetUseLighting(bool bUseLighting);
		void SetLightingParameters(Core::Math::Vec4f cAmbient,
			Core::Math::Vec4f cDiffuse,
			Core::Math::Vec4f cSpecular
			);
		const Core::Math::Vec4f GetAmbientColor();
		const Core::Math::Vec4f GetDiffuseColor();
		const Core::Math::Vec4f GetSpecularColor();
        virtual void SetSampleRateModifier(float fSampleRateModifier);
		void ResetCamera();

        virtual void SetIsoValue(float fIsoValue);
        virtual void SetIsoValueRelative(float fIsovalue);
        virtual float GetIsoValue() const;
        virtual void SetIsosurfaceColor(const Core::Math::Vec3f& vColor);
        virtual Core::Math::Vec3f GetIsosurfaceColor() const;
		void SetColorDataset(bool isColor);

        virtual Core::Math::Vec2ui GetSize() const;

        virtual void Resize(const Core::Math::Vec2ui& vWinSize);

		void SetDataset(Tuvok::IOPtr dio);

        virtual void GetVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const;

        //implement in subclasses
        virtual void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res = false){}; //glrenderer


        //Camera Controls;
        virtual void SetFirstPersonMode(bool mode);
        virtual void RotateCamera(Core::Math::Vec3f rotation);
        virtual void MoveCamera(Core::Math::Vec3f direction);
        virtual void SetCamera(Camera c);
        virtual void SetCameraZoom(float f);


		void SetLoDFactor(const float f);

		float GetSampleRateModifier();

		virtual void ClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues){};

		void SetCompositeMode(ECompositeDisplay displaymode);

		virtual void SetRenderMode(ERenderMode mode);

		virtual void ReadFrameBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount) = 0;
		virtual void Set1DTransferFunction(std::vector<Core::Math::Vec4f> data) = 0;

		virtual Core::Math::Vec4f readVolumePosition(Core::Math::Vec2ui v) = 0;

		void setClearViewEnabled(bool b);

		void setClearViewRadius(float f);

		State getRenderState();

		void setRenderState(State renderState);

		virtual void SwitchPagingStrategy(MissingBrickStrategy brickStrategy) = 0;

		void startRenderThread();
		void pauseRenderThread();
		void stopRenderThread();

      protected:
        virtual void    ScheduleCompleteRedraw();
        void            BuildProjectionMatrix();
        void            BuildViewMatrix();

        virtual bool RecreateAfterResize();

        std::shared_ptr<State>     	m_pRenderState;

        void handleCommands();

        bool				    m_bCompleteRedraw;
        int                     m_iFinishCounter;
        uint64_t                m_iFrameCounter;

        FrameData               m_storedFrame;


      //--------------- private
      private:

        RenderRegionPtr             m_pRenderRegion;
        ExtendedPlanePtr            m_pClipplane;


	  protected:
		Tuvok::IOPtr 				        m_pToCDataset;
		std::vector<std::string>            m_vSearchPathes;
		std::shared_ptr<Context::Context>   m_pContext;
        std::shared_ptr<std::thread>        m_pRenderThread;
        virtual void run() = 0;
        bool                                m_bFinishedInit;

    };

  };
};

#endif
