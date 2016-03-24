#pragma once
#include <memory>

#include "../AbstractRenderer.h"

#include "opengl-base/OpenGLincludes.h"
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
    GridLeaper(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);
    ~GridLeaper();

    virtual void initContext() override;
    virtual void deleteContext() override;

  protected:
    virtual void paintInternal(PaintLevel paintlevel) override;

  private:
    bool loadShaders(GLVolumePool::MissingBrickStrategy brickStrategy);
    void loadGeometry();
    void loadFrameBuffers();
    void loadVolumeData();
    void loadTransferFunction();
    void initHashTable();
    void initVolumePool(uint64_t gpuMemorySizeInByte);

    //help functions move to new GridLeaperTools class?
    Core::Math::Vec3ui CalculateVolumePoolSize(const uint64_t GPUMemorySizeInByte,const uint64_t usedMemory);

    Core::Math::Vec4ui RecomputeBrickVisibility(bool bForceSynchronousUpdate);

    std::vector<Core::Math::Vec4ui8>  m_bufferData;

    std::unique_ptr<GLTexture1D>      m_texTransferFunc;
    std::unique_ptr<GLTexture3D>      m_texVolume;
    std::unique_ptr<GLTargetBinder>   m_targetBinder;

    std::shared_ptr<GLFBOTex>         m_backfaceBuffer;
    std::shared_ptr<GLFBOTex>         m_resultBuffer;
    std::unique_ptr<GLVolumeBox>      m_bbBox;
    std::unique_ptr<GLRenderPlane>    m_nearPlane;
    std::shared_ptr<OpenGlHeadlessContext> m_context;

    std::unique_ptr<GLVolumePool>     m_volumePool;
    std::unique_ptr<GLHashTable>      m_hashTable;

    //Shaders
    std::unique_ptr<GLProgram>        m_programRenderFrontFaces;
    std::unique_ptr<GLProgram>        m_programRenderFrontFacesNearPlane;
    std::unique_ptr<GLProgram>        m_programRayCast1D;
    std::unique_ptr<GLProgram>        m_programRayCast1DColor;
    std::unique_ptr<GLProgram>        m_programRayCast1DLighting;
    std::unique_ptr<GLProgram>        m_programRayCastISO;
    std::unique_ptr<GLProgram>        m_programRayCastISOLighting;
    std::unique_ptr<GLProgram>        m_programRayCastISOColor;
    std::unique_ptr<GLProgram>        m_currentShaderProgram;
    std::unique_ptr<GLProgram>        m_programCompose;
    std::unique_ptr<GLProgram>        m_programComposeColorDebugMix;
    std::unique_ptr<GLProgram>        m_programComposeColorDebugMixAlpha;
    std::unique_ptr<GLProgram>        m_programComposeClearViewIso;

    VisibilityState                   m_VisibilityState;

  };
}
