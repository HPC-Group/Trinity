#pragma once

#include "Connections.h"

#include <QWidget>

#include <string>
#include <vector>

namespace Ui {
class ConnectionWidget;
}

class ConnectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit ConnectionWidget(unsigned int glWidth, unsigned int glHeight, Connections& connections, QWidget* parent = 0);
    ~ConnectionWidget();

private slots:
    void on_connectProc_clicked();
    void on_connectIO_clicked();

    void enableWidgets();

private:
    Ui::ConnectionWidget* m_ui;
    Connections& m_connections;
};