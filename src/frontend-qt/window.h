#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <memory>

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "common/IONodeProxy.h"
#include "frontend-base/ProcessingNodeProxy.h"

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

private slots:
    void on_IOconnectIP_clicked();
    void on_PRconnectIP_clicked();

    void update();
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Ui::Window* ui;
    uint32_t _renderWidth;
    uint32_t _renderHeight;

    int m_mousePosX;
    int m_mousePosY;

    std::unique_ptr<trinity::ProcessingNodeProxy> _processingNode;
    std::unique_ptr<trinity::IONodeProxy> _ioNode;

    std::unique_ptr<trinity::RendererProxy> m_renderer;
    bool _initDone;

    void printDataTree(const std::string& dataId, const std::string& indent = "") const;
};

#endif // WINDOW_H
