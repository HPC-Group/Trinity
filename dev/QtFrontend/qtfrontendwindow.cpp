#include "qtfrontendwindow.h"
#include "ui_qtfrontendwindow.h"

QtFrontendWindow::QtFrontendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFrontendWindow),
	m_selectedDataset(""),
	m_selectedTransferFunction(""),
	m_frameWidth(640),
	m_frameHeight(480)
{
    ui->setupUi(this);

	QTimer* updateTimer = new QTimer(this);
	QObject::connect(updateTimer, SIGNAL(timeout()), this, SLOT(doUpdate()));
	updateTimer->start(30);

    // Create model
    m_modelDatasets = new QStringListModel(this);
	m_modelTransferFunction = new QStringListModel(this);

    m_openGLWidget = new myGLWidget();

    QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_openGLWidget);
    ui->frameRenderWindow->setLayout(layout);

/*	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowErrors(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowMessages(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowWarnings(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowOther(true);*/

	/*LogManager::initialize(LogManager::LogLevel::Debug, true);

	Log* html = new HTMLLog("RenderDemoLog.html");
	Log* text = new TextLog("RenderDemoLog.txt");
	Log* console = new ConsoleLog();
	LogMgr.addLog(html);
	LogMgr.addLog(text);
	LogMgr.addLog(console);

	LINFOC("bla log", "blub");*/

	m_muicontroller = new MUIController();

	connect(ui->actionLoadData, SIGNAL(triggered()), this, SLOT(SelectDataDirectory()));
	connect(ui->listViewDatasets, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(DatasetSelectedSlot(QModelIndex)));
    connect(ui->listViewTransferFunction, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(TransferFunctionSelectedSlot(QModelIndex)));
	
    connect(m_openGLWidget, SIGNAL(Rotate(Core::Math::Vec3f)), this, SLOT(RotateCamera(Core::Math::Vec3f)));
    connect(m_openGLWidget, SIGNAL(Move(Core::Math::Vec3f)), this, SLOT(MoveCamera(Core::Math::Vec3f)));

	connect(m_muicontroller, SIGNAL(Rotate(Core::Math::Vec3f)), this, SLOT(RotateCamera(Core::Math::Vec3f)));

	m_openGLWidget->setSize(m_frameWidth, m_frameHeight);
}

QtFrontendWindow::~QtFrontendWindow()
{
    delete ui;
}

long QtFrontendWindow::milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}

void QtFrontendWindow::InitRenderer()
{
	//IRenderManager& manager = RenderManager::getInstance();
	//renderer = manager.createRenderer("localhost",1234,Visibility::Windowed,Vec2ui(640,480),"WholeBody-SCANLINE-68-lz4.uvf","WholeBody.1dt");
	//m_renderer = manager.createRenderer("localhost", 1234, Visibility::hidden, Vec2ui(m_frameWidth, m_frameHeight), m_selectedDataset.toStdString(), m_selectedTransferFunction.toStdString());
	m_renderer = std::make_shared<NetRendererClient>("localhost", 1234, Visibility::hidden, Vec2ui(m_frameWidth, m_frameHeight), m_selectedDataset.toStdString(), m_selectedTransferFunction.toStdString());

    m_openGLWidget->setRenderer(m_renderer);
}

void QtFrontendWindow::SelectDataDirectory()
{
	m_datasetDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:\Dev\build\trinity\dev\QtFrontend\Release", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	AddDataToModel(m_datasetDirectory, "*.uvf");
	AddDataToModel(m_datasetDirectory, "*.1dt");

	InitRenderer();
}

void QtFrontendWindow::DatasetSelectedSlot(QModelIndex index)
{
	m_selectedDataset = m_modelDatasets->data(index, Qt::DisplayRole).toString();
	m_selectedDataset = m_datasetDirectory + "/" + m_selectedDataset;
}

void QtFrontendWindow::TransferFunctionSelectedSlot(QModelIndex index)
{
	m_selectedTransferFunction = m_modelTransferFunction->data(index, Qt::DisplayRole).toString();
	if (m_selectedTransferFunction != "none")
		m_selectedTransferFunction = m_datasetDirectory + "/" + m_selectedTransferFunction;
}

void QtFrontendWindow::AddDataToModel(QString path, QString extension)
{
    QStringList nameFilter(extension);
    QDir directory(path);
    QStringList files = directory.entryList(nameFilter);

	if (extension == "*.uvf")
	{
		m_modelDatasets->setStringList(files);
		ui->listViewDatasets->setModel(m_modelDatasets);

		m_selectedDataset = files[0];
	}
	else if (extension == "*.1dt")
	{
		files.append("none");
		m_modelTransferFunction->setStringList(files);
		ui->listViewTransferFunction->setModel(m_modelTransferFunction);

		m_selectedTransferFunction = files[0];
	}
}

void  QtFrontendWindow::doUpdate()
{
    if(m_openGLWidget != NULL)
    {
        m_openGLWidget->paintGL();
        m_openGLWidget->repaint();
    }
}

void QtFrontendWindow::RotateCamera(Core::Math::Vec3f rotVec)
{
	if (m_renderer != NULL)
		m_renderer->RotateCamera(rotVec);
}

void QtFrontendWindow::MoveCamera(Core::Math::Vec3f movVec)
{
	if (m_renderer != NULL)
		m_renderer->MoveCamera(movVec);
}
