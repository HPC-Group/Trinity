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

    void setIsoValue(const float) override;
    void initContext() override;
	void deleteContext() override {};  // that does not to go over network... bad design, however
    void zoomCamera(float f) override;
    int getRemoteSessionId() const;
#define PYTHON_MAGIC


#undef PYTHON_MAGIC


private:
    CommandInputChannel m_inputChannel;
    VisStreamReceiver m_visReceiver;
    int m_remoteSessionId;
};
}