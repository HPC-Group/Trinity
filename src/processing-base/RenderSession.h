#pragma once
#include <memory>
#include <string>

#include "mocca/base/Thread.h"
#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"

#include "common/IOSessionProxy.h"
#include "common/IRenderer.h"
#include "common/ISession.h"

#include "VisStreamSender.h"
#include "commands/ICommandFactory.h"


namespace trinity {
namespace processing {


class RenderSession : public common::ISession {

public:
    RenderSession(std::unique_ptr<commands::ICommandFactory> factory, const commands::VclType& rendererType,
                  const commands::StreamingParams& params, const std::string& protocol, std::unique_ptr<common::IOSessionProxy> ioSession);


    ~RenderSession();
    int getVisPort() const;
    common::IRenderer& getRenderer();


private:
    int m_visPort;

    std::unique_ptr<common::IRenderer> m_renderer;
    VisStreamSender m_visSender;

    std::unique_ptr<common::IRenderer> createRenderer(const commands::VclType&,
                                                      std::unique_ptr<common::IOSessionProxy>);
};
}
}