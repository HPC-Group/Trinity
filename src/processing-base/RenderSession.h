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
#include "processing-base/VisStreamSender.h"
#include "processing-base/ProcessingCommandFactory.h"


namespace trinity {
class RenderSession : public AbstractSession {
public:
    RenderSession(const VclType& rendererType, const StreamingParams& params,
                  const std::string& protocol, std::unique_ptr<IOSessionProxy> ioSession);

    ~RenderSession();
    std::string getVisPort() const;
    IRenderer& getRenderer();

private:
    std::unique_ptr<IRenderer> createRenderer(const VclType&, std::unique_ptr<IOSessionProxy>);
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;

private:
    ProcessingSessionCommandFactory m_factory;
    std::string m_visPort;
    std::unique_ptr<IRenderer> m_renderer;
    VisStreamSender m_visSender;
};
}