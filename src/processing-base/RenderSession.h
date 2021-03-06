#pragma once
#include <memory>
#include <string>

#include "mocca/base/Singleton.h"
#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

#include "common/AbstractSession.h"
#include "common/IOSessionProxy.h"
#include "common/IRenderer.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/VisStreamSender.h"

namespace trinity {
class RenderSession : public AbstractSession {
public:
    RenderSession(const VclType& rendererType, const StreamingParams& params, const std::string& protocol,
                  std::unique_ptr<IOSessionProxy> ioSession);
    RenderSession(const std::string& protocol, std::unique_ptr<IRenderer> renderer);
    ~RenderSession();

    std::string getVisPort() const;
    IRenderer& getRenderer();

private:
    void performThreadSpecificTeardown() override;
    void performThreadSpecificInit() override;
    std::unique_ptr<IRenderer> createRenderer(const VclType&, std::unique_ptr<IOSessionProxy>);
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;

private:
    ProcessingSessionCommandFactory m_factory;
    std::string m_visPort;
    std::unique_ptr<IRenderer> m_renderer;
    std::unique_ptr<VisStreamSender> m_visSender;
};
}