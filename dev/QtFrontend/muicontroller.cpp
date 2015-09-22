#include "muicontroller.h"

MUIController::MUIController()
{
	QTimer* updateTimer = new QTimer(this);
	QObject::connect(updateTimer, SIGNAL(timeout()), this, SLOT(loop()));
	updateTimer->start(30);

	//std::string const jsonFilename = configTool.GetJsonFile().c_str();
	//const char* gateConfigFile = jsonFilename.c_str();
	const char* gateConfigFile = "imagevis_body.json";
	if (startService(gateConfigFile) != GATE_OK) {
		std::cout << "Cannot start MUI service: " << lastErrorText() << ")" << std::endl;
		//exit(1);
	}
	else {
		std::cout << "MorphableUI running" << std::endl;
	}
}

MUIController::~MUIController()
{

}

void  MUIController::loop()
{
	unsigned int eventHandle;
	// MORPHABLE UI BEGIN----------------------------------|

	GATE_ERROR_STATE ret = appReceiveEvent(eventHandle, "rotate dataset", "patient");

	if (ret == GATE_OK) {

		float x, y, z;

		eventGetX(eventHandle, x);
		eventGetY(eventHandle, y);
		//eventGetY(eventHandle, z);
		destroyEvent(eventHandle);
		//renderer->SetFirstPersonMode(false);
		//renderer->RotateCamera(Vec3f(x*11.0f, y*11.0f, 0 * 10.0f));
		emit Rotate(Vec3f(-y*11.0f, x*11.0f, 0 * 10.0f));
	}
}

