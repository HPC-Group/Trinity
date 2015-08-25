#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QOpenGLFunctions>
#include <QTimer>

//NETWORKING
#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

//COMPRESSION
#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

#include <renderer/RenderManager.h>
#include <renderer/IRenderer.h>

using namespace mocca;
using namespace mocca::net;
using namespace Tuvok::Renderer;

namespace Ui {
class client;
}

class client : public QMainWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();

    void update();

private:


private slots:
    void on_clearview_clicked();

    void on_slicing_clicked();

    void on_rotation_clicked();

    void requestInUpdate();

private:
    Ui::client *ui;

    QTimer *timer;

    std::unique_ptr<AbstractConnection> clientConnection;

    std::shared_ptr<IRenderer> renderer;
};

#endif // CLIENT_H
