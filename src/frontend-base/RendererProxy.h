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

    void setIsoValue(float) override;
    void initContext() override;
    void deleteContext() override{}; // that does not to go over network... bad design, however
    void zoomCamera(float f) override;
    int getRemoteSessionId() const;
    uint64_t getModalityCount() const override;
    uint64_t getTimestepCount() const override;
    /* AUTOGEN RendererInterfaceOverride */

private:
    CommandInputChannel m_inputChannel;
    VisStreamReceiver m_visReceiver;
    int m_remoteSid;
};
}
