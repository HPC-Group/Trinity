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
    bool connectLoop(trinity::common::IProxy& proxy);
    void initRenderer();

private slots:
    void on_IOconnectIP_clicked();

    void update();

    void on_PRconnectIP_clicked();

private:
    Ui::Window* ui;

    std::unique_ptr<trinity::frontend::ProcessingNodeProxy> _processingNode;
    std::unique_ptr<trinity::common::IONodeProxy> _ioNode;

    std::unique_ptr<trinity::frontend::RendererProxy> _renderer;
};

#endif // WINDOW_H
