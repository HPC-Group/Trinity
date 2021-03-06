#pragma once
#include <memory>

#include "../AbstractRenderer.h"

#include "opengl-base/GLProgram.h"
#include "opengl-base/GLTexture1D.h"
#include "opengl-base/GLTexture3D.h"
#include "opengl-base/GLFrameBufferObject.h"
#include "opengl-base/OpenGlHeadlessContext.h"
#include "opengl-base/GLVolumeBox.h"
#include "opengl-base/ShaderDescriptor.h"
#include "opengl-base/GLTargetBinder.h"
#include "opengl-base/GLRenderPlane.h"
#include "GLVolumePool.h"
#include "GLHashTable.h"
#include "VisibilityState.h"

namespace trinity {
  
  class GridLeaper : public AbstractRenderer {
    
  public:
    GridLeaper(std::shared_ptr<VisStream> stream,
               std::unique_ptr<IIO> ioSession);
    ~GridLeaper();
    
    virtual void initContext() override;
    virtual void deleteContext() override;
    virtual bool isIdle() override;
    virtual bool proceedRendering() override;
    
    bool supportsRenderMode(ERenderMode mode) override;
    
    
    void set1DTransferFunction(const TransferFunction1D& tf) override;
    //void set2DTransferFunction(const TransferFunction2D& tf) override;
    
  protected:
    virtual void paintInternal(PaintLevel paintlevel) override;
    virtual void resizeFramebuffer() override;
    virtual void performClipping() override;
    
  private:
    bool loadShaders(GLVolumePool::MissingBrickStrategy brickStrategy);
    void loadGeometry();
    void initFrameBuffers();
    void loadTransferFunction();
    void initHashTable();
    void initVolumePool(uint64_t gpuMemorySizeInByte);
    
    void fillRayEntryBuffer();
    void raycast();
    void compose();
    void updateBBox();
    
    void computeEyeToModelMatrix();
    
    const uint64_t getFreeGPUMemory();
    
    Core::Math::Vec4ui RecomputeBrickVisibility(bool bForceSynchronousUpdate);
    void swapToNextBuffer();
    void selectShader();
    void setupRaycastShader();
    
    std::unique_ptr<GLTexture1D>      m_texTransferFunc;
    //	std::unique_ptr<GLTexture2D>      m_texTransfer2DFunc;
    
    std::unique_ptr<GLTargetBinder>   m_targetBinder;
    
    
    std::unique_ptr<GLVolumeBox>      m_bBox;
    std::unique_ptr<GLRenderPlane>    m_nearPlane;

    std::unique_ptr<GLVolumePool>     m_volumePool;
    std::unique_ptr<GLHashTable>      m_hashTable;
    
    //Shaders
    std::shared_ptr<GLProgram>        m_activeShaderProgram;
    std::shared_ptr<GLProgram>        m_programRenderFrontFaces;
    std::shared_ptr<GLProgram>        m_programRenderFrontFacesNearPlane;
    std::shared_ptr<GLProgram>        m_programRayCast1D;
    std::shared_ptr<GLProgram>        m_programRayCast1DColor;
    std::shared_ptr<GLProgram>        m_programRayCast1DLighting;
    std::shared_ptr<GLProgram>        m_programRayCast1DLightingColor;
    std::shared_ptr<GLProgram>        m_programRayCast2D;
    std::shared_ptr<GLProgram>        m_programRayCast2DColor;
    std::shared_ptr<GLProgram>        m_programRayCast2DLighting;
    std::shared_ptr<GLProgram>        m_programRayCast2DLightingColor;
    std::shared_ptr<GLProgram>        m_programRayCastISOLighting;
    std::shared_ptr<GLProgram>        m_programRayCastISOColorLighting;
    std::shared_ptr<GLProgram>        m_programCompose;
    std::shared_ptr<GLProgram>        m_programComposeColorDebugMix;
    std::shared_ptr<GLProgram>        m_programComposeColorDebugMixAlpha;
    std::shared_ptr<GLProgram>        m_programComposeClearViewIso;
    
    //Buffers
    std::shared_ptr<GLRenderTexture>       m_resultBuffer;
    std::shared_ptr<GLRenderTexture>       m_pFBORayStart;
    std::shared_ptr<GLRenderTexture>       m_pFBORayStartNext;
    std::shared_ptr<GLRenderTexture>       m_pFBOStartColor;
    std::shared_ptr<GLRenderTexture>       m_pFBOStartColorNext;
    std::shared_ptr<GLRenderTexture>       m_pFBOFinalColor;
    std::shared_ptr<GLRenderTexture>       m_pFBOFinalColorNext;
#ifdef GLGRIDLEAPER_DEBUGVIEW
    std::shared_ptr<GLRenderTexture>       m_pFBODebug;
    std::shared_ptr<GLRenderTexture>       m_pFBODebugNext;
#endif

    std::shared_ptr<OpenGlHeadlessContext> m_context;

    VisibilityState                         m_visibilityState;
    
    float                           m_fLODFactor;
    bool                            m_isIdle;
    
    Core::Math::Mat4f               m_EyeToModelMatrix;
    
    
    
  };
}
