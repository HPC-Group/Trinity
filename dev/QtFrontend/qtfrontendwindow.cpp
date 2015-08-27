#include "qtfrontendwindow.h"
#include "ui_qtfrontendwindow.h"

QtFrontendWindow::QtFrontendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFrontendWindow),
	m_selectedDataset(""),
	m_selectedTransferFunction(""),
	m_frameWidth(640),
	m_frameHeight(480),
	m_componentCount(3),
	m_image(NULL),
	m_pixelmap(NULL),
	m_rawData(NULL),
	m_loop(false)
{
    ui->setupUi(this);

	QTimer* updateTimer = new QTimer(this);
	QObject::connect(updateTimer, SIGNAL(timeout()), this, SLOT(doUpdate()));
	updateTimer->start(30);

    // Create model
    m_modelDatasets = new QStringListModel(this);
	m_modelTransferFunction = new QStringListModel(this);

	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowErrors(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowMessages(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowWarnings(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowOther(true);

	LogManager::initialize(LogManager::LogLevel::Debug, true);

	/*Log* html = new HTMLLog("RenderDemoLog.html");
	Log* text = new TextLog("RenderDemoLog.txt");
	Log* console = new ConsoleLog();
	LogMgr.addLog(html);
	LogMgr.addLog(text);
	LogMgr.addLog(console);

	LINFOC("bla log", "blub");*/

	//m_renderLabel = new RenderLabel("Render window");

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(3, 2);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	m_renderOut = new RendererOut(m_frameWidth, m_frameHeight);

	QVBoxLayout *vbl = new QVBoxLayout();
	vbl->addWidget(m_renderOut);

	ui->frameRenderWindow->setLayout(vbl);

	connect(ui->actionLoadData, SIGNAL(triggered()), this, SLOT(SelectDataDirectory()));
	connect(ui->listViewDatasets, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(DatasetSelectedSlot(QModelIndex)));
    connect(ui->listViewTransferFunction, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(TransferFunctionSelectedSlot(QModelIndex)));

	//m_renderOut->resize(m_frameWidth, m_frameHeight);

	m_renderOut->initRenderServer("walnut.uvf", "none");
}

QtFrontendWindow::~QtFrontendWindow()
{
	//DeleteImage();
	delete m_renderOut;
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

void QtFrontendWindow::SelectDataDirectory()
{
	m_loop = false;

	m_datasetDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:\Dev\build\trinity\dev\QtFrontend\Release", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	AddDataToModel(m_datasetDirectory, "*.uvf");
	AddDataToModel(m_datasetDirectory, "*.1dt");
}

void QtFrontendWindow::DatasetSelectedSlot(QModelIndex index)
{
	m_loop = false;

	m_selectedDataset = m_modelDatasets->data(index, Qt::DisplayRole).toString();
	m_selectedDataset = m_datasetDirectory + "/" + m_selectedDataset;
	//InitRenderer();
}

void QtFrontendWindow::TransferFunctionSelectedSlot(QModelIndex index)
{
	m_loop = false;

	m_selectedTransferFunction = m_modelTransferFunction->data(index, Qt::DisplayRole).toString();
	if (m_selectedTransferFunction != "none")
		m_selectedTransferFunction = m_datasetDirectory + "/" + m_selectedTransferFunction;
	//InitRenderer();
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
	}
	else if (extension == "*.1dt")
	{
		files.append("none");
		m_modelTransferFunction->setStringList(files);
		ui->listViewTransferFunction->setModel(m_modelTransferFunction);
	}
}

void QtFrontendWindow::InitRenderer()
{
	if (m_selectedDataset != "" && m_selectedTransferFunction != "")
	{

		InitImage();

		m_loop = true;
	}
}

void QtFrontendWindow::Loop()
{
	while (true)
	{
		//m_renderLabel->rotateCamera(Vec3f(0.1f*(1 + 1), 0, 0));
		doUpdate();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

void QtFrontendWindow::DeleteImage()
{
	delete m_image;
	delete m_pixelmap;
	delete[] m_rawData;
}

void QtFrontendWindow::InitImage() {
	DeleteImage();

	m_image = new QImage(m_frameWidth, m_frameHeight, QImage::Format_RGB888);
	m_pixelmap = new QPixmap(m_image->width(), m_image->height());
	m_rawData = new uint8_t[3 * m_image->height()*m_image->width()];

	//m_renderLabel->resize(m_frameWidth, m_frameHeight);
}

void  QtFrontendWindow::doUpdate()
{
	m_renderOut->repaint();
	if (m_loop)
	{
		/*FrameData frame = m_renderer->ReadFrameBuffer();

		uint32_t totalSize = 0;

		if (frame._data.size() > 0)
		{
			totalSize = m_frameWidth * m_frameHeight * 3;

			std::copy(&frame._data[0], &frame._data[0] + totalSize, m_image->bits());
			m_renderLabel->setPixmap(QPixmap::fromImage(*m_image));
		}*/
	}
}
