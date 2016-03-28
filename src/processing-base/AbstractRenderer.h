#pragma once

#include <memory>
#include <array>
#include <vector>
#include "common/IRenderer.h"

namespace trinity {

  class AbstractRenderer : public IRenderer {

  public:
    AbstractRenderer(std::shared_ptr<VisStream> stream,
                     std::unique_ptr<IIO> ioSession);
    AbstractRenderer() = default;

    /*******  IRenderer Interface **********/

    // GLOBAL RENDERMODE SETTINGS
    void setRenderMode(ERenderMode mode) override;
    bool supportsRenderMode(ERenderMode mode) override;

    // MODALITIES AND TIMESTEPS
    void setActiveModality(uint64_t modality) override;
    uint64_t getActiveModality() const override;
    void setActiveTimestep(uint64_t timestep) override;
    uint64_t getActiveTimestep() const override;

    uint64_t getModalityCount() const override;
    uint64_t getTimestepCount() const override;
    
    // 1D TF
    void set1DTransferFunction(const TransferFunction1D& tf) override;
    TransferFunction1D getDefault1DTransferFunction(uint64_t index) const override;
    uint64_t getDefault1DTransferFunctionCount() const override;
    std::vector<uint64_t> get1DHistogram() const override;

    /*
     // TODO: 2D TF
     virtual void set2DTransferFunction(const TransferFunction2D& tf);
     virtual TransferFunction2D getDefaul2DTransferFunction(uint64_t index) const;
     virtual uint64_t getDefault2DTransferFunctionCount() const;
     virtual std::vector<uint64_t> get2DHistogram() const;
     */

    // ISO-SURFACE FUNCTIONS
    void setIsoValue(uint8_t surfaceIndex, float fIsoValue) override;
    float getIsoValue(uint8_t surfaceIndex) const override;
    void setIsosurfaceColor(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor) override;
    Core::Math::Vec3ui8 getIsosurfaceColor(uint8_t surfaceIndex) const override;
    Core::Math::Vec2f getRange(uint64_t modality) const override;

    // CLEARVIEW FUNCTIONS
    void setClearViewPosition(const Core::Math::Vec2f& vNormalizedWindowPos) override;
    void setClearViewRadius(float radius) override;
    void setClearBorderSize(float borderSize) override;
    // isovalues and colors are set using the iso functions with surface index 1

    // LIGTHING FUNCTIONS
    virtual void enableLighting(bool enable);
    virtual void setLightingColors(const PhongColorTriple& colors);
    virtual PhongColorTriple getLightingColors() const;
    virtual void setLightDirection(const Core::Math::Vec3f& direction);
    virtual Core::Math::Vec3f getLightDirection() const;

    // SAMPLE RATE FUNCTIONS
    virtual void setSampleRateModifier(float fSampleRateModifier);
    virtual float getSampleRateModifier();

    // render parameters
    virtual void setBoundingBoxMode(BBoxMode mode);
    virtual BBoxMode getBoundingBoxMode() const;

    virtual void setRendererSpecials(const std::vector<uint64_t>& params);
    virtual std::vector<uint64_t> getRendererSpecials() const;


    virtual void setBackgroundColors(std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> colors);
    virtual std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> getBackgroundColors() const;

    // CLIPPING
    virtual void enableClipping(bool enable);
    virtual void setClipVolume(const Core::Math::Vec3f& minValues,
                               const Core::Math::Vec3f& maxValues);

    // TRANSFORMATION
    virtual void setViewParameters(float angle, float znear, float zfar);

    virtual void rotateCamera(Core::Math::Vec3f rotation);
    virtual void moveCamera(Core::Math::Vec3f direction);
    virtual void zoomCamera(float zoom) override;

    virtual void rotateScene(Core::Math::Vec3f rotation);
    virtual void moveScene(Core::Math::Vec3f direction);
    virtual void rescaleScene(float scale);

    virtual void resetCamera();
    virtual void resetObject();

    virtual void startRendering();
    virtual void stopRendering();


    /*******  Non RPC IRenderer Interface **********/

    virtual void initContext() override = 0;
    virtual void deleteContext() override = 0;
    virtual void paint(PaintLevel paintlevel = IRenderer::PaintLevel::PL_REDRAW) {
      if (m_bPaitingActive) paintInternal(paintlevel);
    }
    virtual void resizeFramebuffer();

    /*******  IRenderer Interface end **********/

  protected:
    virtual void paintInternal(PaintLevel paintlevel) = 0;
    virtual void recomputeProjectionMatrix();
    
    ERenderMode                                         m_renderMode;
    uint64_t                                            m_activeModality;
    uint64_t                                            m_activeTimestep;
    TransferFunction1D                                  m_1Dtf;
    //TransferFunction2D                                  m_2Dtf; // TODO: 2D TF
    std::array<float, 2>                                m_isoValue;
    std::array<Core::Math::Vec3ui8, 2>                  m_isoValueColor;
    Core::Math::Vec2f                                   m_clearViewWindowPos;
    float                                               m_clearViewRadius;
    float                                               m_clearBorderSize;
    bool                                                m_enableLighting;
    PhongColorTriple                                    m_lightingColors;
    Core::Math::Vec3f                                   m_lightDirection;
    float                                               m_fSampleRateModifier;
    BBoxMode                                            m_BBoxMode;
    std::vector<uint64_t>                               m_RendererSpecials;
    std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> m_backgroundColors;
    bool                                                m_enableClipping;
    Core::Math::Vec3f                                   m_clipVolumeMin;
    Core::Math::Vec3f                                   m_clipVolumeMax;
    float                                               m_viewAngle;
    float                                               m_zNear;
    float                                               m_zFar;
    Core::Math::Mat4f                                   m_projection;
    Core::Math::Mat4f                                   m_view;
    Core::Math::Mat4f                                   m_model;
    
  private:
    bool    m_bPaitingActive;

    void initValueDefaults();

  };
}
