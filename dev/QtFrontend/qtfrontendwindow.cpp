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

	m_renderLabel = new RenderLabel("Render window");

	QVBoxLayout *vbl = new QVBoxLayout();
	vbl->addWidget(m_renderLabel);

	ui->frameRenderWindow->setLayout(vbl);

	connect(ui->actionLoadData, SIGNAL(triggered()), this, SLOT(SelectDataDirectory()));
	connect(ui->listViewDatasets, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(DatasetSelectedSlot(QModelIndex)));
    connect(ui->listViewTransferFunction, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(TransferFunctionSelectedSlot(QModelIndex)));

	m_renderLabel->resize(m_frameWidth, m_frameHeight);
}

QtFrontendWindow::~QtFrontendWindow()
{
	DeleteImage();

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
	m_datasetDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:\Dev\build\trinity\dev\QtFrontend\Release", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	AddDataToModel(m_datasetDirectory, "*.uvf");
	AddDataToModel(m_datasetDirectory, "*.1dt");
}

void QtFrontendWindow::DatasetSelectedSlot(QModelIndex index)
{
	m_selectedDataset = m_modelDatasets->data(index, Qt::DisplayRole).toString();
	m_selectedDataset = m_datasetDirectory + "/" + m_selectedDataset;
	InitRenderer();
}

void QtFrontendWindow::TransferFunctionSelectedSlot(QModelIndex index)
{
	m_selectedTransferFunction = m_modelTransferFunction->data(index, Qt::DisplayRole).toString();
	m_selectedTransferFunction = m_datasetDirectory + "/" + m_selectedTransferFunction;
	InitRenderer();
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
		m_renderer = RenderManager::getInstance().createRenderer(Visibility::hidden, Vec2ui(m_frameWidth, m_frameHeight), m_selectedDataset.toStdString(), m_selectedTransferFunction.toStdString());
		m_renderer->startRenderThread();

		m_renderLabel->setRenderProtocol(m_renderer);

		InitImage();

		m_loop = true;
		//Loop();
	}
}

void QtFrontendWindow::Loop()
{
	while (true)
	{
		//m_renderer->RotateCamera(Vec3f(0, 0.2f, 0));
		//std::this_thread::sleep_for(std::chrono::milliseconds(30));

		m_renderLabel->rotateCamera(Vec3f(0.1f*(1 + 1), 0, 0));
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

	m_renderLabel->resize(m_frameWidth, m_frameHeight);
}

void  QtFrontendWindow::doUpdate()
{
	if (m_loop)
	{
		long copyTime = milliseconds_now();

		std::vector<uint8_t> image;
		m_renderer->ReadFrameBuffer(image, m_frameWidth, m_frameHeight, m_componentCount);

		std::cout << image.size() << std::endl;
		std::cout << m_componentCount << std::endl;

		uint32_t totalSize = m_image->height() * m_image->bytesPerLine();
		uint8_t* rawDataIter = m_rawData;

		//uint32_t limit = (totalSize / 4);
		uint32_t limit = m_frameWidth * m_frameHeight * m_componentCount;

		if (image.size() > 0)
		{
			for (int i = 0; i < image.size() / 3; i+=3)
			{
				rawDataIter[i] = 255;
				rawDataIter[i + 1] = 0;
				rawDataIter[i + 2] = 0;
			}

			/*for (int i = m_frameHeight - 1; i >= 0; i--)
			{
				for (int j = 0; j < m_frameWidth; j++)
				{
					rawDataIter = image.at((j * m_componentCount) + (i * m_frameWidth * m_componentCount));
					++rawDataIter;
					*rawDataIter = image.at(((j * m_componentCount) + 1) + (i * m_frameWidth * m_componentCount));
					++rawDataIter;
					*rawDataIter = image.at(((j * m_componentCount) + 2) + (i * m_frameWidth * m_componentCount));
					++rawDataIter;
					*rawDataIter = 0;
					++rawDataIter;
				}
			}*/

			totalSize = m_frameWidth * m_frameHeight * 3;

			std::copy(m_rawData, m_rawData + totalSize, m_image->bits());
			m_renderLabel->setPixmap(QPixmap::fromImage(*m_image));
		}
	}
}
