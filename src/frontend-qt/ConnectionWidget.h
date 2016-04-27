#pragma once

#include "commands/Vcl.h"
#include "common/IONodeProxy.h"
#include "frontend-base/ProcessingNodeProxy.h"

#include <QDialog>

#include <string>
#include <vector>

class MainWindow;

namespace Ui {
class ConnectionWidget;
}

class ConnectionWidget : public QDialog {
    Q_OBJECT

public:
    explicit ConnectionWidget(unsigned int glWidth, unsigned int glHeight, trinity::VclType rendererType, QWidget* parent);
    ~ConnectionWidget();

    std::unique_ptr<trinity::RendererProxy> getRenderer();

private slots:
    void on_connectProc_clicked();
    void on_connectIO_clicked();

    void enableWidgets();

private:
    Ui::ConnectionWidget* m_ui;
    trinity::VclType m_rendererType;
    std::unique_ptr<trinity::ProcessingNodeProxy> m_processingNode;
    std::unique_ptr<trinity::IONodeProxy> m_ioNode;
    std::unique_ptr<trinity::RendererProxy> m_renderer;
};