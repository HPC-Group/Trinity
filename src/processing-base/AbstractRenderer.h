#pragma once

#include "common/IRenderer.h"

#include <array>
#include <memory>
#include <vector>

namespace trinity {

  class AbstractRenderer : public IRenderer {

  public:
    AbstractRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);
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
    void enableLighting(bool enable) override;
    void setLightingColors(const PhongColorTriple& colors) override;
    PhongColorTriple getLightingColors() const override;
    void setLightDirection(const Core::Math::Vec3f& direction) override;
    Core::Math::Vec3f getLightDirection() const override;

    // SAMPLE RATE FUNCTIONS
    void setSampleRateModifier(float fSampleRateModifier) override;
    float getSampleRateModifier() const override;

    // render parameters
    void setBoundingBoxMode(BBoxMode mode) override;
    BBoxMode getBoundingBoxMode() const override;

    void setRendererSpecials(const std::vector<uint64_t>& params) override;
    std::vector<uint64_t> getRendererSpecials() const override;

    void setBackgroundColors(const BackgroundColors& colors) override;
    BackgroundColors getBackgroundColors() const override;

    // CLIPPING
    void enableClipping(bool enable) override;
    void setClipVolume(const Core::Math::Vec3f& minValues, const Core::Math::Vec3f& maxValues) override;

    // TRANSFORMATION
    void setViewParameters(float angle, float znear, float zfar) override;

    void rotateCamera(Core::Math::Vec3f rotation) override;
    void moveCamera(Core::Math::Vec3f direction) override;
    void zoomCamera(float zoom) override;

    void rotateScene(Core::Math::Vec3f rotation) override;
    void moveScene(Core::Math::Vec3f direction) override;
    void rescaleScene(float scale) override;

    void resetCamera() override;
    void resetObject() override;

    void startRendering() override;
    void stopRendering() override;

    /*******  Non RPC IRenderer Interface **********/

    virtual void initContext() override = 0;
    virtual void deleteContext() override = 0;
    virtual void paint(PaintLevel paintlevel = IRenderer::PaintLevel::PL_REDRAW) {
      if (m_bPaitingActive)
        paintInternal(paintlevel);
    }
    virtual void resizeFramebuffer() override;

    /*******  IRenderer Interface end **********/

  protected:
    virtual void paintInternal(PaintLevel paintlevel) = 0;

    ERenderMode m_renderMode;
    uint64_t m_activeModality;
    uint64_t m_activeTimestep;
    TransferFunction1D m_1Dtf;
    // TransferFunction2D                                  m_2Dtf; // TODO: 2D TF
    std::array<float, 2> m_isoValue;
    std::array<Core::Math::Vec3ui8, 2> m_isoValueColor;
    Core::Math::Vec2f m_clearViewWindowPos;
    float m_clearViewRadius;
    float m_clearBorderSize;
    bool m_enableLighting;
    PhongColorTriple m_lightingColors;
    Core::Math::Vec3f m_lightDirection;
    float m_fSampleRateModifier;
    BBoxMode m_BBoxMode;
    std::vector<uint64_t> m_RendererSpecials;
    BackgroundColors m_backgroundColors;
    bool m_enableClipping;
    Core::Math::Vec3f m_clipVolumeMin;
    Core::Math::Vec3f m_clipVolumeMax;
    float m_viewAngle;
    float m_zNear;
    float m_zFar;
    
    // derived attributes
    Core::Math::Mat4f m_projection;
    Core::Math::Mat4f m_view;
    Core::Math::Mat4f m_model;
    Core::Math::Mat4f m_modelView;
    float m_1DTFScale;
    float m_2DTFScale;
    Core::Math::Vec3f m_vScale;
    Core::Math::Vec3f m_vExtend;    
    Core::Math::Vec3f m_eyePos;
    
    IIO::ValueType    m_type;
    IIO::Semantic     m_semantic;

    
  private:
    bool m_bPaitingActive;
    
    Core::Math::Mat4f m_camRotation;
    Core::Math::Mat4f m_camTranslation;
    Core::Math::Mat4f m_camZoom;
    Core::Math::Mat4f m_rotation;
    Core::Math::Mat4f m_translation;
    Core::Math::Mat4f m_scale;

    void recomputeProjectionMatrix();
    void recomputeModelViewMatrix();

    void initValueDefaults();
    
    void calculateDerived();
    void calculate1DTFDerived();
  };
}
