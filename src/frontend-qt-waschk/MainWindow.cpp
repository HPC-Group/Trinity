#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commands/Vcl.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>

using namespace std;
using namespace mocca::net;
using namespace trinity;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_rendererType(VclType::GridLeapingRenderer)
    , m_iCurrent1DIndex(0) {
    m_ui->setupUi(this);
    this->setWindowTitle("Trinty Demo");;
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::update() {
    auto& visStream = *m_renderer->getVisStream();
    int resX = visStream.getStreamingParams().getResX();
    int resY = visStream.getStreamingParams().getResY();
    try {
        auto frame = visStream.get();
        if (!frame.empty()) {
            m_ui->openGLWidget->setData(resX, resY, frame.data());
            m_ui->openGLWidget->repaint();
        }
        m_renderer->proceedRendering();
    } catch (const mocca::net::ConnectionClosedError&) {
        resetRenderer();
        QMessageBox::warning(this, "Connection Closed", "Connection to processing node has been lost.");
    }
}

void MainWindow::on_actionTrinity_triggered() {
    resetRenderer();
    ConnectionWidget dlg(m_ui->openGLWidget->width(), m_ui->openGLWidget->height(), m_rendererType, this);
    dlg.move(geometry().x(), geometry().y() + m_ui->toolBar->height());
    dlg.exec();
    m_renderer = std::move(dlg.getRenderer());
    if (m_renderer) {
        connect(&m_repaintTimer, SIGNAL(timeout()), this, SLOT(update()));
        m_repaintTimer.start(10);
        m_renderer->startRendering();
    }
}

void MainWindow::on_actionPrev_triggered() {
    const uint64_t maxIndex = m_renderer->getDefault1DTransferFunctionCount() - 1;
    if (m_iCurrent1DIndex > 0) {
        m_iCurrent1DIndex--;
    } else {
        m_iCurrent1DIndex = maxIndex;
    }

    m_renderer->set1DTransferFunction(m_renderer->getDefault1DTransferFunction(m_iCurrent1DIndex));
}

void MainWindow::on_actionNext_triggered() {
    const uint64_t maxIndex = m_renderer->getDefault1DTransferFunctionCount() - 1;
    if (m_iCurrent1DIndex < maxIndex) {
        m_iCurrent1DIndex++;
    } else {
        m_iCurrent1DIndex = 0;
    }

    m_renderer->set1DTransferFunction(m_renderer->getDefault1DTransferFunction(m_iCurrent1DIndex));
}

void MainWindow::wheelEvent(QWheelEvent* event) {
    if (!m_renderer) {
        return;
    }

    static float isoValue = 0;
    isoValue += event->delta() / 2400.f;
    m_renderer->setIsoValue(0, isoValue);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!m_renderer) {
        return;
    }

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

void MainWindow::on_actionToggleRenderer_triggered() {
    if (m_rendererType == VclType::SimpleRenderer) {
        m_rendererType = VclType::GridLeapingRenderer;
        m_ui->actionToggleRenderer->setText(QString("current renderer : GridLeaper"));
    } else {
        m_rendererType = VclType::SimpleRenderer;
        m_ui->actionToggleRenderer->setText(QString("current renderer : SimpleRenderer"));
    }
}

void MainWindow::resetRenderer() {
    m_repaintTimer.stop();
    m_renderer = nullptr;
}
