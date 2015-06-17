//
//  IRenderer.h
//  TNG
//
//  Created by Andre Waschk on 22/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef TNG_IRenderer_h
#define TNG_IRenderer_h


#include <core/Math/Vectors.h>
#include <memory.h>
#include "CullingLOD.h"

#include "RenderEnums.h"
#include "Service/RenderStatev2.h"

#include <renderer/Context/Context.h>

#include <thread>

namespace Tuvok{
  class IDataIO;
  typedef std::shared_ptr<IDataIO> DataIOPtr;



  namespace IO{
    class Dataset;
    typedef std::shared_ptr<Dataset> DatasetPtr;
  }

  class RenderRegion;
  typedef std::shared_ptr<RenderRegion> RenderRegionPtr;

  class ExtendedPlane;
  typedef std::shared_ptr<ExtendedPlane> ExtendedPlanePtr;

  class Context; // wofür context class bei opengl ?
  typedef std::shared_ptr<Context> ContextPtr;

  namespace Renderer{
    class Camera;
    typedef std::shared_ptr<Camera> CameraPtr;

    class IRenderer{
    public:
      IRenderer(){};
      virtual ~IRenderer(){};

      virtual bool Paint();

        virtual void SetClearFrameBuffer(bool bClearFramebuffer) = 0;
        virtual bool GetClearFrameBuffer() const = 0;

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

        virtual void SetIsoValue(float fIsoValue) = 0;
        virtual void SetIsoValueRelative(float fIsovalue) = 0;
        virtual float GetIsoValue() const = 0;
        virtual void SetIsosurfaceColor(const Core::Math::Vec3f& vColor) = 0;
        virtual Core::Math::Vec3f GetIsosurfaceColor() const = 0;
		virtual void SetColorDataset(bool isColor) = 0;

        virtual Core::Math::Vec2ui GetSize() const = 0;

        virtual void Resize(const Core::Math::Vec2ui& vWinSize) = 0;

        virtual void GetVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const = 0;

        //implement in subclasses
        virtual void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res = false) = 0; //glrenderer


        //Camera Controls;
        virtual void SetFirstPersonMode(bool mode) = 0;
        virtual void RotateCamera(Core::Math::Vec3f rotation) = 0;
        virtual void MoveCamera(Core::Math::Vec3f direction) = 0;
        virtual void SetCamera(Camera c) = 0;
        virtual void SetCameraZoom(float f) = 0;


		virtual void SetLoDFactor(const float f) = 0;

		virtual float GetSampleRateModifier() = 0;

		virtual void ClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues) = 0;

		virtual void SetCompositeMode(ECompositeDisplay displaymode) = 0;

		virtual void SetRenderMode(ERenderMode mode) = 0;

		virtual void ReadFrameBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount) = 0;
		virtual void Set1DTransferFunction(std::vector<Core::Math::Vec4f> data) = 0;

		virtual Core::Math::Vec4f readVolumePosition(Core::Math::Vec2ui v) = 0;

		virtual void setClearViewEnabled(bool b) = 0;

		virtual void setClearViewRadius(float f) = 0;

		virtual State getRenderState() = 0;

		virtual void setRenderState(State renderState) = 0;




		virtual void SwitchPagingStrategy(MissingBrickStrategy brickStrategy) = 0;


		virtual void startRenderThread() = 0;
		virtual void pauseRenderThread() = 0;
		virtual void stopRenderThread() = 0;

		protected:
        virtual bool RecreateAfterResize() = 0;
    };

  };
};

#endif
