#pragma once

#include "frontend-base/ProcessingNodeProxy.h"

#include "ConnectionWidget.h"

#include <QMainWindow>
#include <QTimer>

#include <memory>

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
    void resetRenderer();

private:
    Ui::MainWindow* m_ui;
    trinity::VclType m_rendererType;
    QTimer m_repaintTimer;
    std::unique_ptr<trinity::RendererProxy> m_renderer;
    int m_mousePosX;
    int m_mousePosY;
    uint64_t m_iCurrent1DIndex;
};