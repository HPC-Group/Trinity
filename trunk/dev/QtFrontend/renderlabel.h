#ifndef RENDERLABEL_H
#define RENDERLABEL_H

#include <renderer\RenderManager.h>
#include <renderer\AbstrRenderer.h>

#include <QMouseEvent>
#include <QKeyEvent>
#include <qlabel.h>

#include <core\Math/Vectors.h>

#include <iostream>

using namespace Tuvok::Renderer;
using namespace Core::Math;

class RenderLabel : public QLabel
{

Q_OBJECT

public:
	explicit RenderLabel(const QString& text = "", QWidget * parent = 0);
    ~RenderLabel();

	void setRenderProtocol(std::shared_ptr<IRenderer> renderer);
	void setFrameSize(int width, int height);
	void rotateCamera(Vec3f rot);

signals:
	void clicked();

public slots:
	void slotClicked();

protected:
	//void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void keyPressEvent(QKeyEvent * event);

private:
	Vec2d m_LastMouseClick;

	std::shared_ptr<IRenderer> renderProtocol;

	Vec2d m_frameSize;
};

#endif // RENDERLABEL_H
