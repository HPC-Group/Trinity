#include "ConnectionWidget.h"
#include "ui_ConnectionWidget.h"

#include "IODataModel.h"
#include "ConnectionSinglton.h"
#include "MainWindow.h"

#include <QMessageBox>

#define io ConnectionSingleton::getInstance().ioNode()

ConnectionWidget::ConnectionWidget(unsigned int glWidth, unsigned int glHeight, QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::ConnectionWidget) {
    m_ui->setupUi(this);
    show();

    m_ui->widthProc->setText(QString(std::to_string(glWidth).c_str()));
    m_ui->heightProc->setText(QString(std::to_string(glHeight).c_str()));
}

ConnectionWidget::~ConnectionWidget() {
    delete m_ui;
}

void ConnectionWidget::on_io_connect_clicked() {
    try {
        ConnectionSingleton::getInstance().connectIO(m_ui->hostnameIO->text().toStdString(), m_ui->portIO->text().toStdString());
        m_ui->fileView->setModel(new IODataModel(*ConnectionSingleton::getInstance().ioNode(), this));
        connect(m_ui->fileView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this,
                SLOT(enableWidgets()));
    } catch (const std::runtime_error& err) {
        this->close();
        QMessageBox::critical(this, "Error", err.what());
    }
}

void ConnectionWidget::on_proc_connect_clicked() {
    try {
        ConnectionSingleton::getInstance().connectProcessing(m_ui->hostnameProc->text().toStdString(),
                                                             m_ui->portProc->text().toStdString());

        std::string dataID = m_ui->fileView->currentIndex().data(Qt::UserRole + 1).value<trinity::IOData*>()->getFileId();

        ConnectionSingleton::getInstance().initRenderer(m_ui->hostnameIO->text().toStdString(), m_ui->portIO->text().toStdString(),
                                                        m_ui->widthProc->text().toInt(), m_ui->heightProc->text().toInt(), dataID);

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
    m_ui->connectProc->setEnabled(io != nullptr && !hasChildren && ioData != nullptr);
}