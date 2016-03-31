#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>

#include "mocca/log/LogManager.h"

#include "commands/Vcl.h"
#include "common/TrinityError.h"
#include "connectionSinglton.h"


using namespace std;
using namespace mocca::net;
using namespace trinity;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(nullptr),
    _initDone(false),
    m_renderer(nullptr),
    m_renderWidth(800),
    m_renderHeight(600)
{
    ui->setupUi(this);
    this->setWindowTitle("Trinty Demo");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}


static float rot = 0.0f;
void MainWindow::update() {
    if(!_initDone)
        return;

    auto frameNullable = m_renderer->getVisStream()->get();
    if (!frameNullable.isNull()) {
        auto frame = frameNullable.release();
        ui->openGLWidget->setData(m_renderWidth,
                                  m_renderHeight,
                                  frame.data());
        ui->openGLWidget->repaint();
    }
    m_renderer->proceedRendering();
}

void MainWindow::on_actionTrinity_triggered()
{
    settings = std::make_unique<connectionSettings>(ui->openGLWidget->width(),
                                                    ui->openGLWidget->height(),
                                                    this);
}

void MainWindow::on_actionPrev_triggered()
{

}

void MainWindow::on_actionNext_triggered()
{

}


void MainWindow::wheelEvent(QWheelEvent* event) {
    if (!_initDone)
        return;

    static float isoValue = 0;
    isoValue += event->delta() / 2400.f;
    m_renderer->setIsoValue(0, isoValue);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {

    if (!_initDone)
        return;

    float deltaX = (m_mousePosX - event->localPos().x()) / 200.f;
    float deltaY = (m_mousePosY - event->localPos().y()) / 200.f;
    if (event->buttons() & Qt::RightButton) {
        Core::Math::Vec3f vec(-deltaX, deltaY, .0f);
        m_renderer->moveCamera(vec);
    } else if (event->buttons() & Qt::LeftButton) {
        Core::Math::Vec3f vec(-deltaY, -deltaX, .0f);
        m_renderer->rotateScene(vec);
    }
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::initRenderer(  const std::string& iohostname,
                                   const std::string& ioport,
                                   const uint32_t width,
                                   const uint32_t height,
                                   const std::string fileId){

    trinity::StreamingParams params(width, height);
    m_renderWidth = width;
    m_renderHeight = height;

    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(),
                        iohostname,
                        ioport);

    // the file id will be available after implementing the listdata command
    try {
      m_renderer = ConnectionSingleton::getInstance().processingNode().initRenderer(trinity::VclType::SimpleRenderer,
                                                fileId, endpointIO, params);
      //_renderer->initContext();
    } catch (const trinity::TrinityError&) {
      LERROR("(qt) no connection to a renderer");
    }

    m_renderer->startRendering();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    _initDone = true;
}
