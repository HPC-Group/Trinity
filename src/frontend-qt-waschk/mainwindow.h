#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "connectionsettings.h"

#include "frontend-base/ProcessingNodeProxy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private slots:
    void update();

    void on_actionTrinity_triggered();

    void on_actionPrev_triggered();

    void on_actionNext_triggered();

    void on_actionToggleRenderer_triggered();

private:
    Ui::MainWindow* m_ui;
    std::unique_ptr<connectionSettings> m_settings;
    int m_mousePosX;
    int m_mousePosY;
    uint64_t m_iCurrent1DIndex;
};

#endif // MAINWINDOW_H
