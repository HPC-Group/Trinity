#ifndef QTFRONTENDWINDOW_H
#define QTFRONTENDWINDOW_H

//logger, use sweden logger later ! will remove debugouthandle
#include <tools/DebugOutHandler.h>

#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QStringListModel>
#include <qtimer.h>
#include <qsurfaceformat.h>

#include "RendererOut.h"

using namespace ghoul::logging;

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

protected slots:
	void doUpdate();

private:
    Ui::QtFrontendWindow *ui;

	long milliseconds_now();
    void AddDataToModel(QString path, QString extension);
	void InitRenderer();
	void Loop();

	void DeleteImage();
	void InitImage();

	QPixmap* m_pixelmap;
	QImage* m_image;
	uint8_t* m_rawData;

	bool m_loop;

	int m_frameWidth;
	int m_frameHeight;
	int m_componentCount;

	QString m_datasetDirectory;
	QString m_transferFunctionDirectory;
    QStringListModel *m_modelDatasets;
	QStringListModel *m_modelTransferFunction;

	QString m_selectedDataset;
	QString m_selectedTransferFunction;

	//RenderLabel *m_renderLabel;
	RendererOut *m_renderOut;
};

#endif // QTFRONTENDWINDOW_H
