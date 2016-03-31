#include "connectionsettings.h"
#include "ui_connectionsettings.h"

#include "connectionSinglton.h"
#include "mainwindow.h"
#include "IODataModel.h"

#define io ConnectionSingleton::getInstance().ioNode()


connectionSettings::connectionSettings(unsigned int glWidth,
                                       unsigned int glHeight,
                                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connectionSettings),
    m_activeFileId("null")
{
    ui->setupUi(this);
    show();

    ui->proc_width->setText(QString(std::to_string(glWidth).c_str()));
    ui->proc_height->setText(QString(std::to_string(glHeight).c_str()));
}

connectionSettings::~connectionSettings()
{
    delete ui;
}

void connectionSettings::on_io_connect_clicked()
{
    ConnectionSingleton::getInstance().connectIO(ui->io_hostname->text().toStdString(),
                                                 ui->io_port->text().toStdString());

    //printDataTree("FractalData");


    ui->fileView->setModel(new IODataModel(ConnectionSingleton::getInstance().ioNode(), this));
    connect(ui->fileView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(enableWidgets()));
}

void connectionSettings::on_proc_connect_clicked()
{
    ConnectionSingleton::getInstance().connectProcessing(ui->proc_hostname->text().toStdString(),
                                                            ui->proc_port->text().toStdString());

    std::string dataID = ui->fileView->currentIndex().data(Qt::UserRole + 1).value<trinity::IOData*>()->getFileId();

    ConnectionSingleton::getInstance().initRenderer(    ui->io_hostname->text().toStdString(),
                           ui->io_port->text().toStdString(),
                           ui->proc_width->text().toInt(),
                           ui->proc_height->text().toInt(),
                           dataID);


    this->close();
}
