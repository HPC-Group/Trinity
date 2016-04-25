#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>

#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"

#include "commands/Vcl.h"
#include "common/TrinityError.h"
#include "connectionSinglton.h"

using namespace std;
using namespace mocca::net;
using namespace trinity;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_settings(nullptr)
    , m_iCurrent1DIndex(0) {
    m_ui->setupUi(this);
    this->setWindowTitle("Trinty Demo");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

static float rot = 0.0f;
void MainWindow::update() {
    if (!ConnectionSingleton::getInstance().isReady()) {
        return;
    }

    auto& renderer = ConnectionSingleton::getInstance().renderer();
    try {
        auto frame = renderer.getVisStream()->get();
        if (!frame.empty()) {
            m_ui->openGLWidget->setData(renderer.getVisStream()->getStreamingParams().getResX(),
                                        renderer.getVisStream()->getStreamingParams().getResY(), frame.data());
            m_ui->openGLWidget->repaint();
        }
        renderer.proceedRendering();
    } catch (const mocca::net::ConnectionClosedError&) {
        QMessageBox::warning(this, "Connection Closed", "Connection to processing node has been lost.");
        ConnectionSingleton::getInstance().reset();
    }
}

void MainWindow::on_actionTrinity_triggered() {
    m_settings = mocca::make_unique<ConnectionWidget>(m_ui->openGLWidget->width(), m_ui->openGLWidget->height(), this);
}

void MainWindow::on_actionPrev_triggered() {
    auto& renderer = ConnectionSingleton::getInstance().renderer();
    const uint64_t maxIndex = renderer.getDefault1DTransferFunctionCount() - 1;
    if (m_iCurrent1DIndex > 0) {
        m_iCurrent1DIndex--;
    } else {
        m_iCurrent1DIndex = maxIndex;
    }

    renderer.set1DTransferFunction(renderer.getDefault1DTransferFunction(m_iCurrent1DIndex));
}

void MainWindow::on_actionNext_triggered() {
    auto& renderer = ConnectionSingleton::getInstance().renderer();
    const uint64_t maxIndex = renderer.getDefault1DTransferFunctionCount() - 1;
    if (m_iCurrent1DIndex < maxIndex)
        m_iCurrent1DIndex++;
    else
        m_iCurrent1DIndex = 0;

    renderer.set1DTransferFunction(renderer.getDefault1DTransferFunction(m_iCurrent1DIndex));
}

void MainWindow::wheelEvent(QWheelEvent* event) {
    if (!ConnectionSingleton::getInstance().isReady()) {
        return;
    }

    static float isoValue = 0;
    isoValue += event->delta() / 2400.f;
    auto& renderer = ConnectionSingleton::getInstance().renderer();
    renderer.setIsoValue(0, isoValue);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!ConnectionSingleton::getInstance().isReady()) {
        return;
    }

    float deltaX = (m_mousePosX - event->localPos().x()) / 200.f;
    float deltaY = (m_mousePosY - event->localPos().y()) / 200.f;
    if (event->buttons() & Qt::RightButton) {
        Core::Math::Vec3f vec(-deltaX, deltaY, .0f);
        auto& renderer = ConnectionSingleton::getInstance().renderer();
        renderer.moveCamera(vec);
    } else if (event->buttons() & Qt::LeftButton) {
        Core::Math::Vec3f vec(-deltaY, -deltaX, .0f);
        auto& renderer = ConnectionSingleton::getInstance().renderer();
        renderer.rotateScene(vec);
    }
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::on_actionToggleRenderer_triggered() {
    if (ConnectionSingleton::getInstance().getRendererType() == trinity::VclType::SimpleRenderer) {
        ConnectionSingelton.setRendererType(trinity::VclType::GridLeapingRenderer);
        m_ui->actionToggleRenderer->setText(QString("current renderer : GridLeaper"));
    } else {
        ConnectionSingelton.setRendererType(trinity::VclType::SimpleRenderer);
        m_ui->actionToggleRenderer->setText(QString("current renderer : SimpleRenderer"));
    }
}
