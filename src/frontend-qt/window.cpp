#include "window.h"
#include "ui_window.h"
#include <QTimer>

#include <iostream>

#include "mocca/log/LogManager.h"

#include "commands/Vcl.h"

using namespace std;
using namespace mocca::net;

static int reconnectInSec = 5;

Window::Window(QWidget* parent)
    : QMainWindow(parent),
      _initDone(false),
      _renderWidth(512),
      _renderHeight(512),
      ui(new Ui::Window)
{
    ui->setupUi(this);
    LINFO("Window created");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

Window::~Window() {
    delete ui;
}

bool Window::connectLoop(trinity::common::IProxy& proxy) {
    bool connected = false;
    int attempts = 10;

    while (!connected && attempts > 0) {
        connected = proxy.connect();
        if (!connected) {
            attempts--;
            LINFO("reconnecting to io in " << std::to_string(reconnectInSec) << " seconds");
            std::this_thread::sleep_for(std::chrono::seconds(reconnectInSec));
        }
    }
    return connected;
}

void Window::initRenderer() {
    trinity::commands::StreamingParams params(_renderWidth, _renderHeight);

    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");

    // the file id will be available after implementing the listdata command
    int fileId = 0;
    _renderer = _processingNode->initRenderer(trinity::commands::VclType::GridLeaper,
                                                 fileId,
                                                 endpointIO,
                                                 params);
    _renderer->connect();

    // sending commands
    _renderer->initContext();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    _initDone = true;
}

void Window::on_IOconnectIP_clicked() {
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");

    _ioNode = std::unique_ptr<trinity::common::IONodeProxy>(new trinity::common::IONodeProxy(endpointIO));
    if (!connectLoop(*_ioNode)) {
        LINFO("could not connect, handle the errors later etc.");
        return;
    }
}

void Window::on_PRconnectIP_clicked()
{
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), "localhost", "5678");

    _processingNode = std::unique_ptr<trinity::frontend::ProcessingNodeProxy>
    (new trinity::frontend::ProcessingNodeProxy(endpoint));
    if(!connectLoop(*_processingNode))
    {
        LINFO("could not connect, handle the errors later etc.");
        return;
    }

    LINFO("connected to processing node");

    initRenderer();

}
static float rot = 0.0f;
void Window::update(){
    if(_initDone){
        _renderer->setIsoValue(rot);
        rot += 0.01f;

        auto frame = _renderer->getVisStream()->get();
        if(frame){
            ui->openGLWidget->setData(_renderWidth,_renderHeight,frame->data());
            ui->openGLWidget->repaint();
        }
    }
}

void Window::repaint(){
}
