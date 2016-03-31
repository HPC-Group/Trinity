#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QWidget>
#include <vector>
#include <string>

namespace Ui {
class connectionSettings;
}

class connectionSettings : public QWidget
{
    Q_OBJECT

public:
    explicit connectionSettings(unsigned int glWidth = 800,
                                unsigned int glHeight = 600,
                                QWidget *parent = 0);
    ~connectionSettings();

private slots:
    void on_io_connect_clicked();

    void on_proc_connect_clicked();

private:
    Ui::connectionSettings      *ui;
    std::vector<std::string>    m_fileIds;
    std::string                 m_activeFileId;

};

#endif // CONNECTIONSETTINGS_H
