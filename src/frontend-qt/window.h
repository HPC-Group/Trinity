#ifndef WINDOW_H
#define WINDOW_H

#include "common/IONodeProxy.h"
#include "frontend-base/ProcessingNodeProxy.h"

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include <QMainWindow>

#include <memory>

namespace Ui {
class Window;
}

class Window : public QMainWindow {
    Q_OBJECT

public:
    explicit Window(QWidget* parent = 0);
    ~Window();
    void repaint();

private:
    void initRenderer();
    void printDataTree(const std::string& dataId, const std::string& indent = "") const;

private slots:
    void on_IOconnectIP_clicked();
    void on_PRconnectIP_clicked();

    void update();
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void enableWidgets();

private:
    Ui::Window* m_ui;
    uint32_t m_renderWidth;
    uint32_t m_renderHeight;

    int m_mousePosX;
    int m_mousePosY;

    std::unique_ptr<trinity::ProcessingNodeProxy> m_processingNode;
    std::unique_ptr<trinity::IONodeProxy> m_ioNode;

    std::unique_ptr<trinity::RendererProxy> m_renderer;
    bool m_initDone;
};

#endif // WINDOW_H
