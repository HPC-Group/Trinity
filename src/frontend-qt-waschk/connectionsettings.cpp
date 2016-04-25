#include "connectionsettings.h"
#include "ui_connectionsettings.h"

#include "IODataModel.h"
#include "connectionSinglton.h"
#include "mainwindow.h"

#include <QMessageBox>

#define io ConnectionSingleton::getInstance().ioNode()

connectionSettings::connectionSettings(unsigned int glWidth, unsigned int glHeight, QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::connectionSettings) {
    m_ui->setupUi(this);
    show();

    m_ui->proc_width->setText(QString(std::to_string(glWidth).c_str()));
    m_ui->proc_height->setText(QString(std::to_string(glHeight).c_str()));
}

connectionSettings::~connectionSettings() {
    delete m_ui;
}

void connectionSettings::on_io_connect_clicked() {
    try {
        ConnectionSingleton::getInstance().connectIO(m_ui->io_hostname->text().toStdString(), m_ui->io_port->text().toStdString());
        m_ui->fileView->setModel(new IODataModel(*ConnectionSingleton::getInstance().ioNode(), this));
        connect(m_ui->fileView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this,
                SLOT(enableWidgets()));
    } catch (const std::runtime_error& err) {
        this->close();
        QMessageBox::critical(this, "Error", err.what());
    }
}

void connectionSettings::on_proc_connect_clicked() {
    try {
        ConnectionSingleton::getInstance().connectProcessing(m_ui->proc_hostname->text().toStdString(),
                                                             m_ui->proc_port->text().toStdString());

        std::string dataID = m_ui->fileView->currentIndex().data(Qt::UserRole + 1).value<trinity::IOData*>()->getFileId();

        ConnectionSingleton::getInstance().initRenderer(m_ui->io_hostname->text().toStdString(), m_ui->io_port->text().toStdString(),
                                                        m_ui->proc_width->text().toInt(), m_ui->proc_height->text().toInt(), dataID);

        this->close();
    } catch (const std::runtime_error& err) {
        this->close();
        QMessageBox::critical(this, "Error", err.what());
    }
}

void connectionSettings::enableWidgets() {
    auto index = m_ui->fileView->currentIndex();
    bool hasChildren = m_ui->fileView->model()->hasChildren(index);
    auto ioData = index.data(Qt::UserRole + 1).value<trinity::IOData*>();
    m_ui->proc_connect->setEnabled(io != nullptr && !hasChildren && ioData != nullptr);
}