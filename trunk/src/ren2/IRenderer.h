//
//  IRenderer.h
//  TNG
//
//  Created by Andre Waschk on 22/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef TNG_IRenderer_h
#define TNG_IRenderer_h

#include <Core/Math/Vectors.h>
#include <memory.h>

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

      virtual bool Initialize()= 0;
      virtual void Cleanup()= 0;
      virtual bool Paint()= 0;

      virtual void SetClearFrameBuffer(bool bClearFramebuffer)= 0;
      virtual bool GetClearFrameBuffer() const = 0;

      virtual void SetViewParameters(float angle, float znear, float zfar,
                                     const Core::Math::Vec3f& eye,
                                     const Core::Math::Vec3f& ref,
                                     const Core::Math::Vec3f& vup)= 0;

      virtual void SetViewPos(const Core::Math::Vec3f& vPos)= 0;
      virtual Core::Math::Vec3f GetViewPos() const= 0;
      virtual void ResetViewPos()= 0;
      virtual void SetViewDir(const Core::Math::Vec3f& vDir)= 0;
      virtual Core::Math::Vec3f GetViewDir() const = 0;
      virtual void ResetViewDir()= 0;
      virtual void SetUpDir(const Core::Math::Vec3f& vDir) = 0;
      virtual Core::Math::Vec3f GetUpDir() const = 0;
      virtual void ResetUpDir()= 0;

      virtual Core::Math::Mat4f& GetProjectionMatrix()= 0;
      virtual Core::Math::Mat4f& GetViewMatrix()= 0;

      virtual bool SetBackgroundColors(Core::Math::Vec3f vTopColor,
                                       Core::Math::Vec3f vBottomColor)= 0;
      virtual Core::Math::Vec3f GetBackgroundColor(const uint8_t index) = 0;

      virtual void SetUseLighting(bool bUseLighting)= 0;
      virtual void SetSampleRateModifier(float fSampleRateModifier)= 0;

      virtual void SetIsoValue(float fIsoValue)= 0;
      virtual void SetIsoValueRelative(float fIsovalue)= 0;
      virtual void SetIsosurfaceColor(const Core::Math::Vec3f& vColor)= 0;
      virtual Core::Math::Vec3f GetIsosurfaceColor() const = 0;

      virtual Core::Math::Vec2ui GetSize() const = 0;

      virtual void Resize(const Core::Math::Vec2ui& vWinSize)= 0;

      virtual bool LoadFile(const std::string& filename)= 0;
      virtual void SetDataset(IO::DatasetPtr vds)= 0;
      virtual void ClearBricks()= 0;
      virtual bool LoadRebricked(const std::string& strFilename,
                                 const Core::Math::Vec3ui bsize,
                                 size_t minmaxMode)= 0;
      //NEW DATAIO LAYER
      virtual void SetDataset(DataIOPtr dio) = 0;
      virtual DataIOPtr GetDataIO() const = 0;

      virtual void GetVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const = 0;

      //implement in subclasses
      virtual void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                               bool decrease_screen_res = false)= 0; //glrenderer

      //Camera Controls;
      virtual void SetFirstPersonMode(bool mode) = 0;
      virtual void RotateCamera(Core::Math::Vec3f rotation) = 0;
      virtual void MoveCamera(Core::Math::Vec3f direction) = 0;
      virtual void SetCameraZoom(float f) = 0;



      //not implemented yet or clueless :D ----------------------------------------
      virtual void SetRotationRR(RenderRegionPtr region,
                                 const Core::Math::Mat4f rotation)= 0; //not sure

      virtual const Core::Math::Mat4f GetRotation(RenderRegionPtr region)= 0; //not sure

      virtual void SetTranslation(RenderRegionPtr region,
                                  const Core::Math::Mat4f translation)= 0; //not sure

      virtual const Core::Math::Mat4f GetTranslation(RenderRegionPtr) const= 0; //not sure

      virtual void SetClipPlane(RenderRegionPtr region,
                                ExtendedPlanePtr plane)= 0; //not sure

      virtual bool IsClipPlaneEnabled(RenderRegionPtr renderRegion=NULL)= 0; //not sure
      virtual void EnableClipPlane(RenderRegionPtr renderRegion=NULL)= 0; //not sure
      virtual void DisableClipPlane(RenderRegionPtr renderRegion=NULL)= 0; //not sure

      virtual void Set2DFlipMode(RenderRegionPtr region, bool bFlipX, bool bFlipY)= 0; //not sure
      virtual void Get2DFlipMode(RenderRegionPtr region, bool& bFlipX, bool& bFlipY)= 0; //not sure

      virtual Core::Math::Vec2ui GetCVMousePos() const = 0; //what is clear view ?
      virtual Core::Math::Vec3f Pick(const Core::Math::Vec2ui)= 0; //not sure

      virtual void SheduleWindowRedraw(RenderRegionPtr region)= 0; //not sure


      virtual void SetUserMatrices(const Core::Math::Mat4f& view, const Core::Math::Mat4f& projection)= 0; //needed ?
      virtual void UnsetUserMatrices()= 0; //needed ?

      virtual void SetScalingMethod()= 0; //check enum

      virtual ContextPtr getContext() const = 0; //why context ? what ?


      virtual void SetFrustumCullingModelMatrix(const Core::Math::Mat4f& modelMatrix)= 0; // not yet
      virtual int GetFrustumCullingLODLevel(const Core::Math::Mat4f& vfCenter,
                                            const Core::Math::Mat4f& vfExtent,
                                            const Core::Math::Mat4f& viVoxelCount) const = 0; //not yet

      virtual void SetConsiderPreviousDepthbuffer(bool bConsiderPreviousDepthbuffer) = 0; //not yet;
      virtual bool GetConsiderPreviousDepthbuffer() const = 0; // not yet
    };

  };
};

#endif
