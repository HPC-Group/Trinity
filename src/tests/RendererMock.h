#include "gmock/gmock.h"

#include "common/IRenderer.h"

class RendererMock : public trinity::IRenderer {
public:
  MOCK_METHOD1(setRenderMode, void(IRenderer::ERenderMode));
  MOCK_METHOD1(supportsRenderMode, bool(IRenderer::ERenderMode));
  MOCK_METHOD0(resizeFramebuffer, void());
  MOCK_METHOD0(initContext, void());
  MOCK_METHOD0(deleteContext, void());
  MOCK_METHOD1(setActiveModality, void(uint64_t));
  MOCK_CONST_METHOD0(getActiveModality, uint64_t());
  MOCK_METHOD1(setActiveTimestep, void(uint64_t));
  MOCK_CONST_METHOD0(getActiveTimestep, uint64_t());
  MOCK_CONST_METHOD0(getModalityCount, uint64_t());
  MOCK_CONST_METHOD0(getTimestepCount, uint64_t());
  MOCK_METHOD1(set1DTransferFunction, void(const TransferFunction1D&));
  MOCK_CONST_METHOD1(getDefault1DTransferFunction, TransferFunction1D(uint64_t));
  MOCK_CONST_METHOD0(getDefault1DTransferFunctionCount, uint64_t());
  MOCK_CONST_METHOD0(get1DHistogram, std::vector<uint64_t>());
  MOCK_METHOD2(setIsoValue, void(uint8_t, float));
  MOCK_CONST_METHOD1(getIsoValue, float(uint8_t));
  MOCK_METHOD2(setIsosurfaceColor, void(uint8_t, const Core::Math::Vec3ui8&));
  MOCK_CONST_METHOD1(getIsosurfaceColor, Core::Math::Vec3ui8(uint8_t));
  MOCK_CONST_METHOD1(getRange, Core::Math::Vec2f(uint64_t));
  MOCK_METHOD1(setClearViewPosition, void(const Core::Math::Vec2f&));
  MOCK_METHOD1(setClearViewRadius, void(float));
  MOCK_METHOD1(setClearBorderSize, void(float));
  MOCK_METHOD1(enableLighting, void(bool));
  MOCK_METHOD1(setLightingColors, void(const PhongColorTriple&));
  MOCK_CONST_METHOD0(getLightingColors, PhongColorTriple());
  MOCK_METHOD1(setLightDirection, void(const Core::Math::Vec3f&));
  MOCK_CONST_METHOD0(getLightDirection, Core::Math::Vec3f());
  MOCK_METHOD1(setSampleRateModifier, void(float));
  MOCK_CONST_METHOD0(getSampleRateModifier, float());
  MOCK_METHOD1(setBoundingBoxMode, void(BBoxMode));
  MOCK_CONST_METHOD0(getBoundingBoxMode, BBoxMode());
  MOCK_METHOD1(setRendererSpecials, void(const std::vector<uint64_t>&));
  MOCK_CONST_METHOD0(getRendererSpecials, std::vector<uint64_t>());
  MOCK_METHOD1(setBackgroundColors, void(const BackgroundColors&));
  MOCK_CONST_METHOD0(getBackgroundColors, BackgroundColors());
  MOCK_METHOD1(enableClipping, void(bool));
  MOCK_METHOD2(setClipVolume, void(const Core::Math::Vec3f&, const Core::Math::Vec3f&));
  MOCK_METHOD3(setViewParameters, void(float, float, float));
  MOCK_METHOD1(rotateCamera, void(Core::Math::Vec3f));
  MOCK_METHOD1(moveCamera, void(Core::Math::Vec3f));
  MOCK_METHOD1(zoomCamera, void(float));
  MOCK_METHOD1(rotateScene, void(Core::Math::Vec3f));
  MOCK_METHOD1(moveScene, void(Core::Math::Vec3f));
  MOCK_METHOD1(rescaleScene, void(float));
  MOCK_METHOD0(resetCamera, void());
  MOCK_METHOD0(resetObject, void());
  MOCK_METHOD0(startRendering, void());
  MOCK_METHOD0(stopRendering, void());
  MOCK_METHOD0(isIdle, bool());
  MOCK_METHOD0(proceedRendering, bool());
};