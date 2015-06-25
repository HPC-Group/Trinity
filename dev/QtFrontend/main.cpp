#include "qtfrontendwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFrontendWindow w;
    w.show();

    return a.exec();
}
