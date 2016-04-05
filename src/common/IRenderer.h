#pragma once

#include "common/IIO.h"

#include "mocca/base/BidirectionalMap.h"

#include <memory>
#include <vector>

namespace trinity {
  class VisStream;
  class IRenderer {

  public:
    enum class ERenderMode {
      RM_1DTRANS = 0,
      RM_2DTRANS,
      RM_ISOSURFACE,
      RM_CLEARVIEW,
      RM_INVALID
    };

    enum class BBoxMode {
      BBM_NONE = 0,
      BBM_DATASET,
      BBM_BRICKS
    };

    enum class PaintLevel {
      PL_REDRAW_VISIBILITY_CHANGE = 0,
      PL_REDRAW,
      PL_CONTINUE,
      PL_RECOMPOSE
    };

    using RenderModeMapper = mocca::BidirectionalMap<ERenderMode, std::string>;
    static const RenderModeMapper& renderModeMapper();

    using BBoxModeMapper = mocca::BidirectionalMap<BBoxMode, std::string>;
    static const BBoxModeMapper& bboxModeMapper();

    struct PhongColorTriple : public SerializableTemplate<PhongColorTriple> {
      PhongColorTriple() = default;
      PhongColorTriple(const Core::Math::Vec4ui8& a,
                       const Core::Math::Vec4ui8& d,
                       const Core::Math::Vec4ui8& s)
      : ambient(a)
      , diffuse(d)
      , specular(s) {}

      Core::Math::Vec4ui8 ambient;
      Core::Math::Vec4ui8 diffuse;
      Core::Math::Vec4ui8 specular;

      bool equals(const PhongColorTriple& other) const;
      std::string toString() const;

      void serialize(ISerialWriter& writer) const override;
      void deserialize(const ISerialReader& reader) override;
    };

    struct BackgroundColors : public SerializableTemplate<BackgroundColors> {
      BackgroundColors() = default;
      BackgroundColors(const Core::Math::Vec3ui8& c1, const Core::Math::Vec3ui8& c2)
      : colorOne(c1)
      , colorTwo(c2) {}

      Core::Math::Vec3ui8 colorOne;
      Core::Math::Vec3ui8 colorTwo;

      bool equals(const BackgroundColors& other) const;
      std::string toString() const;

      void serialize(ISerialWriter& writer) const override;
      void deserialize(const ISerialReader& reader) override;
    };

    IRenderer() = default;

    // all renderers need a vis stream
    IRenderer(std::shared_ptr<VisStream> s);

    // "real" renderers, in addition, need an I/O interface
    IRenderer(std::shared_ptr<VisStream> s, std::unique_ptr<IIO> io);

    virtual ~IRenderer(){};

    std::shared_ptr<VisStream> getVisStream();

    // GLOBAL RENDERMODE SETTINGS
    virtual void setRenderMode(ERenderMode mode) = 0;
    virtual bool supportsRenderMode(ERenderMode mode) = 0;

    // MODALITIES AND TIMESTEPS
    virtual void setActiveModality(uint64_t modality) = 0;
    virtual uint64_t getActiveModality() const = 0;
    virtual void setActiveTimestep(uint64_t timestep) = 0;
    virtual uint64_t getActiveTimestep() const = 0;

    virtual uint64_t getModalityCount() const = 0;
    virtual uint64_t getTimestepCount() const = 0;

    // 1D TF
    virtual void set1DTransferFunction(const TransferFunction1D& tf) = 0;
    virtual TransferFunction1D getDefault1DTransferFunction(uint64_t index) const = 0;
    virtual uint64_t getDefault1DTransferFunctionCount() const = 0;
    virtual std::vector<uint64_t> get1DHistogram() const = 0;

    // ISO-SURFACE FUNCTIONS
    virtual void setIsoValue(uint8_t surfaceIndex, float) = 0;
    virtual float getIsoValue(uint8_t surfaceIndex) const = 0;
    virtual void setIsosurfaceColor(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor) = 0;
    virtual Core::Math::Vec3ui8 getIsosurfaceColor(uint8_t surfaceIndex) const = 0;
    virtual Core::Math::Vec2f getRange(uint64_t modality) const = 0;

    // CLEARVIEW FUNCTIONS
    virtual void setClearViewPosition(const Core::Math::Vec2f& vNormalizedWindowPos) = 0;
    virtual void setClearViewRadius(float radius) = 0;
    virtual void setClearBorderSize(float borderSize) = 0;

    // LIGTHING FUNCTIONS
    virtual void enableLighting(bool enable) = 0;
    virtual void setLightingColors(const PhongColorTriple& colors) = 0;
    virtual PhongColorTriple getLightingColors() const = 0;
    virtual void setLightDirection(const Core::Math::Vec3f& direction) = 0;
    virtual Core::Math::Vec3f getLightDirection() const = 0;

    // SAMPLE RATE FUNCTIONS
    virtual void setSampleRateModifier(float fSampleRateModifier) = 0;
    virtual float getSampleRateModifier() const = 0;

    // render parameters
    virtual void setBoundingBoxMode(BBoxMode mode) = 0;
    virtual BBoxMode getBoundingBoxMode() const = 0;

    virtual void setRendererSpecials(const std::vector<uint64_t>& params) = 0;
    virtual std::vector<uint64_t> getRendererSpecials() const = 0;

    virtual void setBackgroundColors(const BackgroundColors& colors) = 0;
    virtual BackgroundColors getBackgroundColors() const = 0;

    // CLIPPING
    virtual void enableClipping(bool enable) = 0;
    virtual void setClipVolume(const Core::Math::Vec3f& minValues,
                               const Core::Math::Vec3f& maxValues) = 0;

    // TRANSFORMATION
    virtual void setViewParameters(float angle, float znear, float zfar) = 0;

    virtual void setUserViewMatrix(Core::Math::Mat4f m) = 0;
    virtual void setUserWorldMatrix(Core::Math::Mat4f m) = 0;

    virtual void rotateCamera(Core::Math::Vec3f rotation) = 0;
    virtual void moveCamera(Core::Math::Vec3f direction) = 0;
    virtual void zoomCamera(float zoom) = 0;

    virtual void rotateScene(Core::Math::Vec3f rotation) = 0;
    virtual void moveScene(Core::Math::Vec3f direction) = 0;
    virtual void rescaleScene(float scale) = 0;

    virtual void resetCamera() = 0;
    virtual void resetObject() = 0;

    virtual void startRendering() = 0;
    virtual bool proceedRendering() = 0;
    virtual void stopRendering() = 0;

    virtual bool isIdle() = 0;

    // postponed init and delete due to opengl specific things
    virtual void initContext() = 0;
    virtual void deleteContext() = 0;
    virtual void resizeFramebuffer() = 0;

  protected:
    std::shared_ptr<VisStream> m_visStream;
    std::unique_ptr<IIO> m_io;
  };

  bool operator==(const IRenderer::PhongColorTriple& lhs, const IRenderer::PhongColorTriple& rhs);
  std::ostream& operator<<(std::ostream& os, const IRenderer::PhongColorTriple& obj);

  bool operator==(const IRenderer::BackgroundColors& lhs, const IRenderer::BackgroundColors& rhs);
  std::ostream& operator<<(std::ostream& os, const IRenderer::BackgroundColors& obj);
}