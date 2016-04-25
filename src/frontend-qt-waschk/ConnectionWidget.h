#pragma once

#include <QWidget>

#include <string>
#include <vector>

namespace Ui {
class ConnectionWidget;
}

class ConnectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit ConnectionWidget(unsigned int glWidth = 800, unsigned int glHeight = 600, QWidget* parent = 0);
    ~ConnectionWidget();

private slots:
    void on_io_connect_clicked();
    void on_proc_connect_clicked();

    void enableWidgets();

private:
    Ui::ConnectionWidget* m_ui;
};