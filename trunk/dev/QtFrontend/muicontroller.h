#ifndef MUICONTROLLER_H
#define MUICONTROLLER_H

#include <QObject>
#include <QTimer>

#include <core/Math/Vectors.h>
#include <gateapi.h>

using namespace Core::Math;

class MUIController : public QObject
{
    Q_OBJECT
public:
    MUIController();
    ~MUIController();

signals:
	void Rotate(Core::Math::Vec3f);
	void Move(Core::Math::Vec3f);

protected slots:
	void loop();
};

#endif // MUICONTROLLER_H
