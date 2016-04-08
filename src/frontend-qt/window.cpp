#include "window.h"

#include "frontend-qt/IODataModel.h"
#include "ui_window.h"

#include "commands/Vcl.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"

#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>

#include <iostream>

using namespace std;
using namespace mocca::net;

static int reconnectInSec = 5;

Window::Window(QWidget* parent)
    : QMainWindow(parent)
    , m_initDone(false)
    , m_ui(new Ui::Window) {
    m_ui->setupUi(this);
    LINFO("Window created");
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    connect(m_ui->IOconnectIP, SIGNAL(clicked()), this, SLOT(enableWidgets()));
}

Window::~Window() {
    delete m_ui;
}

void Window::printDataTree(const std::string& dataId, const std::string& indent) const {
    try {
        if (indent.empty()) {
            LINFO("Listing " + dataId);
        }

        std::vector<trinity::IOData> list = m_ioNode->listFiles(dataId);
        for (auto e : list) {
            if (e.getDataType() == trinity::IOData::DataType::Directory) {
                LINFO(indent + "Directory:" + e.getName() + "\t (" + e.getFileId() + ")");
                printDataTree(e.getFileId(), indent + "  ");
            } else {
                LINFO(indent + e.getName() + "\t (" + e.getFileId() + ")");
            }
        }
    } catch (const trinity::TrinityError&) {
    }
}

void Window::initRenderer() {
    m_renderWidth = (m_ui->resx->text().toInt());
    m_renderHeight = (m_ui->resy->text().toInt());
    trinity::StreamingParams params(m_renderWidth, m_renderHeight);

    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), m_ui->IOaddressIPedit->text().toStdString(),
                        m_ui->IOaddressPortedit->text().toStdString());

    printDataTree("FractalData");
    printDataTree("UVFData");

    auto dataID = m_ui->dsWidget->currentIndex().data(Qt::UserRole + 1).value<trinity::IOData*>()->getFileId();

    try {
        m_renderer = m_processingNode->initRenderer(trinity::VclType::SimpleRenderer, dataID, endpointIO, params);
    } catch (const trinity::TrinityError&) {
        LERROR("(qt) no connection to a renderer");
    }

    // set initial parameters here

    // TODO

    // initial parameter end

    m_renderer->startRendering();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_initDone = true;
}

void Window::on_IOconnectIP_clicked() {
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), m_ui->IOaddressIPedit->text().toStdString(),
                        m_ui->IOaddressPortedit->text().toStdString());
    m_ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
    m_ui->dsWidget->setModel(new IODataModel(*m_ioNode, this));
    connect(m_ui->dsWidget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(enableWidgets()));
}

void Window::on_PRconnectIP_clicked() {
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), m_ui->PRaddressIPedit->text().toStdString(),
                      m_ui->PRaddressPortedit->text().toStdString());

    m_processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
    LINFO("connected to processing node");
    initRenderer();
}

void Window::update() {
    if (m_initDone && m_renderer) {
        auto frame = m_renderer->getVisStream()->get();
        if (frame != nullptr) {
            m_ui->openGLWidget->setData(m_renderWidth, m_renderHeight, frame->data());
            m_ui->openGLWidget->repaint();
        }
        m_renderer->proceedRendering();
    }
}

void Window::repaint() {}

void Window::wheelEvent(QWheelEvent* event) {
    if (!m_initDone)
        return;

    static float isoValue = 0;
    isoValue += event->delta() / 2400.f;
    m_renderer->setIsoValue(0, isoValue);
}

void Window::mousePressEvent(QMouseEvent* event) {
    m_mousePosX = event->localPos().x();
    m_mousePosY = event->localPos().y();
}

void Window::mouseMoveEvent(QMouseEvent* event) {
    if (!m_initDone)
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

void Window::enableWidgets() {
    auto index = m_ui->dsWidget->currentIndex();
    bool hasChildren = m_ui->dsWidget->model()->hasChildren(index);
    auto ioData = index.data(Qt::UserRole + 1).value<trinity::IOData*>();
    m_ui->PRconnectIP->setEnabled(m_ioNode != nullptr && !hasChildren && ioData != nullptr);
}