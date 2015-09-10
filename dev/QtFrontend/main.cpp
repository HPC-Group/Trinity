#include "qtfrontendwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setVersion(2, 1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	QSurfaceFormat::setDefaultFormat(format);

    QtFrontendWindow w;
    w.show();

    return a.exec();
}
