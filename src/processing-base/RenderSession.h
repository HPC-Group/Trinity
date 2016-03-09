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
#include "common/SessionManager.h"

#include "VisStreamSender.h"
#include "commands/ICommandFactory.h"


namespace trinity {
class RenderSession : public AbstractSession {
public:
    RenderSession(std::unique_ptr<ICommandFactory> factory, const VclType& rendererType, const StreamingParams& params,
                  const std::string& protocol, std::unique_ptr<IOSessionProxy> ioSession);

    ~RenderSession();
    int getVisPort() const;
    IRenderer& getRenderer();

private:
    int m_visPort;

    std::unique_ptr<IRenderer> m_renderer;
    VisStreamSender m_visSender;

    std::unique_ptr<IRenderer> createRenderer(const VclType&, std::unique_ptr<IOSessionProxy>);
};

using RenderSessionManager = mocca::Singleton<SessionManager<RenderSession>>;
}