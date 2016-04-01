#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>
#include <QMessageBox>

#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"

#include "commands/Vcl.h"
#include "common/TrinityError.h"
#include "connectionSinglton.h"

#define renderer ConnectionSingleton::getInstance().renderer()
#define ready ConnectionSingleton::getInstance().isReady()

using namespace std;
using namespace mocca::net;
using namespace trinity;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(nullptr) {
    ui->setupUi(this);
    this->setWindowTitle("Trinty Demo");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

MainWindow::~MainWindow() {
    delete ui;
}

static float rot = 0.0f;
void MainWindow::update() {
    if (!ready) {
        return;
    }

    try {
        auto frameNullable = renderer.getVisStream()->get();
        if (!frameNullable.isNull()) {
            auto frame = frameNullable.release();
            ui->openGLWidget->setData(renderer.getVisStream()->getStreamingParams().getResX(),
                renderer.getVisStream()->getStreamingParams().getResY(), frame.data());
            ui->openGLWidget->repaint();
        }
        renderer.proceedRendering();
    }
    catch (const mocca::net::ConnectionClosedError&) {
        QMessageBox::warning(this, "Connection Closed", "Connection to processing node has been lost.");
        ConnectionSingleton::getInstance().reset();
    }
}

void MainWindow::on_actionTrinity_triggered() {
    settings = mocca::make_unique<connectionSettings>(ui->openGLWidget->width(), ui->openGLWidget->height(), this);
}

void MainWindow::on_actionPrev_triggered() {}

void MainWindow::on_actionNext_triggered() {}

void MainWindow::wheelEvent(QWheelEvent* event) {
    if (!ready) {
        return;
    }

    static float isoValue = 0;
    isoValue += event->delta() / 2400.f;
    renderer.setIsoValue(0, isoValue);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!ready) {
        return;
    }

    float deltaX = (m_mousePosX - event->localPos().x()) / 200.f;
    float deltaY = (m_mousePosY - event->localPos().y()) / 200.f;
    if (event->buttons() & Qt::RightButton) {
        Core::Math::Vec3f vec(-deltaX, deltaY, .0f);
        renderer.moveCamera(vec);
    } else if (event->buttons() & Qt::LeftButton) {
        Core::Math::Vec3f vec(-deltaY, -deltaX, .0f);
        renderer.rotateScene(vec);
    }
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::on_actionToggleRenderer_triggered() {
    if (ConnectionSingleton::getInstance().getRendererType() == trinity::VclType::SimpleRenderer) {
        ConnectionSingelton.setRendererType(trinity::VclType::GridLeapingRenderer);
        ui->actionToggleRenderer->setText(QString("current renderer : GridLeaper"));
    } else {
        ConnectionSingelton.setRendererType(trinity::VclType::SimpleRenderer);
        ui->actionToggleRenderer->setText(QString("current renderer : SimpleRenderer"));
    }
}
