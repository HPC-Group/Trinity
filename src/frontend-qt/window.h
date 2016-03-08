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

    void update();

    void on_PRconnectIP_clicked();

private:
    Ui::Window* ui;
    uint32_t _renderWidth;
    uint32_t _renderHeight;

    std::unique_ptr<trinity::ProcessingNodeProxy> _processingNode;
    std::unique_ptr<trinity::IONodeProxy> _ioNode;

    std::unique_ptr<trinity::RendererProxy> _renderer;
    bool _initDone;
};

#endif // WINDOW_H
