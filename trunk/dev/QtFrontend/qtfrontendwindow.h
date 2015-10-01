#ifndef QTFRONTENDWINDOW_H
#define QTFRONTENDWINDOW_H

//logger, use sweden logger later ! will remove debugouthandle
//#include <tools\DebugOutHandler.cpp>

//#include "logging/logmanager.h"
//#include "logging/consolelog.h"
//#include "logging/htmllog.h"
//#include "logging/textlog.h"

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QStringListModel>
#include <qtimer.h>
#include <qsurfaceformat.h>

#include <renderer/IRenderer.h>
//#include <renderer\RenderManager.cpp>
#include <NetRendererClient.h>

#ifdef DENABLEMUI
	#include "muicontroller.h"
#endif



//using namespace ghoul::logging;
using namespace Tuvok::Renderer;

namespace Ui {
class QtFrontendWindow;
}

class QtFrontendWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtFrontendWindow(QWidget *parent = 0);
    ~QtFrontendWindow();

public slots:
    void DatasetSelectedSlot(QModelIndex);
	void TransferFunctionSelectedSlot(QModelIndex);
	void SelectDataDirectory();
	void RotateCamera(Core::Math::Vec3f rotVec);
	void MoveCamera(Core::Math::Vec3f movVec);

protected slots:
	void doUpdate();

private:
    Ui::QtFrontendWindow *ui;

	//long milliseconds_now();
    void AddDataToModel(QString path, QString extension);
	void InitRenderer();

	int m_frameWidth;
	int m_frameHeight;

	QString m_datasetDirectory;
	QString m_transferFunctionDirectory;
    QStringListModel *m_modelDatasets;
	QStringListModel *m_modelTransferFunction;

	QString m_selectedDataset;
	QString m_selectedTransferFunction;

	std::shared_ptr<IRenderer> m_renderer;

#ifdef DENABLEMUI
	MUIController *m_muicontroller;
#endif
};

#endif // QTFRONTENDWINDOW_H
