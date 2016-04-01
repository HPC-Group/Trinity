#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QWidget>
#include <string>
#include <vector>

namespace Ui {
class connectionSettings;
}

class connectionSettings : public QWidget {
    Q_OBJECT

public:
    explicit connectionSettings(unsigned int glWidth = 800, unsigned int glHeight = 600, QWidget* parent = 0);
    ~connectionSettings();

private slots:
    void on_io_connect_clicked();
    void on_proc_connect_clicked();

    void enableWidgets();

private:
    Ui::connectionSettings* ui;
};

#endif // CONNECTIONSETTINGS_H
