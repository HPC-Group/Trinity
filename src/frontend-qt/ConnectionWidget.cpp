#include "ConnectionWidget.h"
#include "ui_ConnectionWidget.h"

#include "IODataModel.h"
#include "MainWindow.h"

#include "mocca/base/Memory.h"

#include <QMessageBox>

using namespace mocca::net;

ConnectionWidget::ConnectionWidget(unsigned int glWidth, unsigned int glHeight, trinity::VclType rendererType, QWidget* parent)
    : QDialog(parent)
    , m_rendererType(rendererType)
    , m_ui(new Ui::ConnectionWidget) {
    m_ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    m_ui->widthProc->setText(QString(std::to_string(glWidth).c_str()));
    m_ui->heightProc->setText(QString(std::to_string(glHeight).c_str()));
}

ConnectionWidget::~ConnectionWidget() {
    delete m_ui;
}

std::unique_ptr<trinity::RendererProxy> ConnectionWidget::getRenderer() {
    return std::move(m_renderer);
}

void ConnectionWidget::on_connectIO_clicked() {
    try {
        std::string hostname = m_ui->hostnameIO->text().toStdString();
        std::string port = m_ui->portIO->text().toStdString();
        Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), hostname, port);
        m_ioNode = mocca::make_unique<trinity::IONodeProxy>(endpointIO);
        m_ui->fileView->setModel(new IODataModel(*m_ioNode, this));
        connect(m_ui->fileView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this,
                SLOT(enableWidgets()));
    } catch (const std::runtime_error& err) {
        this->close();
        QMessageBox::critical(this, "Error", err.what());
    }
}

void ConnectionWidget::on_connectProc_clicked() {
    try {
        // create processing node proxy
        std::string hostname = m_ui->hostnameProc->text().toStdString();
        std::string port = m_ui->portProc->text().toStdString();
        Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), hostname, port);
        m_processingNode = mocca::make_unique<trinity::ProcessingNodeProxy>(endpoint);

        // create renderer proxy
        std::string hostnameIO = m_ui->hostnameIO->text().toStdString();
        std::string portIO = m_ui->portIO->text().toStdString();
        std::string dataID = m_ui->fileView->currentIndex().data(Qt::UserRole + 1).value<trinity::IOData*>()->getFileId();
        int renderWidth = m_ui->widthProc->text().toInt();
        int renderHeight = m_ui->heightProc->text().toInt();
        trinity::StreamingParams params(renderWidth, renderHeight);
        Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), hostnameIO, portIO);
        m_renderer = m_processingNode->initRenderer(m_rendererType, dataID, endpointIO, params);
        this->close();
    } catch (const std::runtime_error& err) {
        this->close();
        QMessageBox::critical(this, "Error", err.what());
    }
}

void ConnectionWidget::enableWidgets() {
    auto index = m_ui->fileView->currentIndex();
    bool hasChildren = m_ui->fileView->model()->hasChildren(index);
    auto ioData = index.data(Qt::UserRole + 1).value<trinity::IOData*>();
    m_ui->connectProc->setEnabled(m_ioNode != nullptr && !hasChildren && ioData != nullptr);
}