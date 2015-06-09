#ifndef TNG_AbstrRenderer_h
#define TNG_AbstrRenderer_h

#include "IRenderer.h"

#include <core/Math/Vectors.h>
#include <memory.h>
#include "CullingLOD.h"

#include "RenderEnums.h"
#include "Service/RenderStatev2.h"

namespace Tuvok{
  class IOLocal;
  typedef std::shared_ptr<IOLocal> IOPtr;

  namespace Renderer{

      class AbstrRenderer{
	  public:

      public:
		  AbstrRenderer(Core::Math::Vec2ui vWinSize = Core::Math::Vec2ui(640, 480), ERenderMode mode = RM_ISOSURFACE);
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
		void SetColorDataset(bool isColor){
			m_pRenderState->m_bIsColor = isColor;
		};

        virtual Core::Math::Vec2ui GetSize() const;

        virtual void Resize(const Core::Math::Vec2ui& vWinSize);

        virtual bool LoadFile(const std::string& filename);
        virtual void ClearBricks();
        virtual bool LoadRebricked(const std::string& strFilename,
                                   const Core::Math::Vec3ui bsize,
                                   size_t minmaxMode);

        virtual void SetDataset(DataIOPtr dio);
		void SetDataset(Tuvok::IOPtr dio);
        virtual DataIOPtr GetDataIO() const;

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

		void setClearViewEnabled(bool b){
            		m_pRenderState->m_bClearViewEnabled = b;
            		this->ScheduleCompleteRedraw();
		}

		void setClearViewRadius(float f){
			m_pRenderState->m_fClearViewRadius = f;
			this->ScheduleCompleteRedraw();
		}

		State getRenderState(){
			return *(m_pRenderState.get());
		}

		void setRenderState(State renderState){
            m_pRenderState.reset();
            m_pRenderState = std::make_shared<State>(renderState);
		}

      protected:
        virtual void ScheduleCompleteRedraw();
        void BuildProjectionMatrix();
        void BuildViewMatrix();

	virtual bool RecreateAfterResize();

	std::shared_ptr<State>     	m_pRenderState;

	bool				m_bCompleteRedraw;

      //--------------- private
      private:

        RenderRegionPtr             m_pRenderRegion;
        ExtendedPlanePtr            m_pClipplane;

        DataIOPtr                   m_pDatasetIO;


	protected:
		Tuvok::IOPtr 				m_pToCDataset;
    };

  };
};

#endif
