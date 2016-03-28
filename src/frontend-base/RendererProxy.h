#pragma once
#include <memory>

#include "common/IRenderer.h"
#include "commands/CommandInputChannel.h"
#include "frontend-base/VisStreamReceiver.h"

namespace trinity {
class RendererProxy : public IRenderer {
public:
    /// local proxy to a remote render session
    RendererProxy(std::shared_ptr<VisStream> stream, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                  unsigned int sessionId);

    virtual ~RendererProxy();

    void setRenderMode(ERenderMode mode) override;
    bool supportsRenderMode(ERenderMode mode) override;

    void setActiveModality(uint64_t modality) override;
    uint64_t getActiveModality() const override;
    void setActiveTimestep(uint64_t timestep) override;
    uint64_t getActiveTimestep() const override;

    void setIsoValue(uint8_t, float) override;
    void initContext() override;
    void deleteContext() override{}; // that does not to go over network... bad design, however
    void zoomCamera(float f) override;
    int getRemoteSessionId() const;
    uint64_t getModalityCount() const override;
    uint64_t getTimestepCount() const override;
    void set1DTransferFunction(const TransferFunction1D& tf) override;
    TransferFunction1D getDefault1DTransferFunction(uint64_t index) const override;
    uint64_t getDefault1DTransferFunctionCount() const override;
    std::vector<uint64_t> get1DHistogram() const override;
    float getIsoValue(uint8_t surfaceIndex) const override;
    void setIsosurfaceColor(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor) override;
    Core::Math::Vec3ui8 getIsosurfaceColor(uint8_t surfaceIndex) const override;
    Core::Math::Vec2f getRange(uint64_t modality) const override;
    void setClearViewPosition(const Core::Math::Vec2f& vNormalizedWindowPos) override;
    void setClearViewRadius(float radius) override;
    void setClearBorderSize(float borderSize) override;
    void enableLighting(bool enable) override;
    void setLightingColors(const PhongColorTriple& colors) override;
    PhongColorTriple getLightingColors() const override;
    void setLightDirection(const Core::Math::Vec3f& direction) override;
    Core::Math::Vec3f getLightDirection() const override;
    void setSampleRateModifier(float fSampleRateModifier) override;
    float getSampleRateModifier() const override;
    void setBoundingBoxMode(BBoxMode mode) override;
    BBoxMode getBoundingBoxMode() const override;
    void setRendererSpecials(const std::vector<uint64_t>& params) override;
    std::vector<uint64_t> getRendererSpecials() const override;
    void setBackgroundColors(const BackgroundColors& colors) override;
    BackgroundColors getBackgroundColors() const override;
    void enableClipping(bool enable) override;
    void setClipVolume(const Core::Math::Vec3f& minValues, const Core::Math::Vec3f& maxValues) override;
    /* AUTOGEN RendererInterfaceOverride */

private:
    CommandInputChannel m_inputChannel;
    VisStreamReceiver m_visReceiver;
    int m_remoteSid;
};
}
