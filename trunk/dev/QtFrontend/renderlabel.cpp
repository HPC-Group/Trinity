#include "renderlabel.h"

RenderLabel::RenderLabel(const QString& text, QWidget * parent)
	: QLabel(parent)
{
	this->setText(text);
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));

	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
}

RenderLabel::~RenderLabel()
{

}

void RenderLabel::slotClicked()
{
	
}

void RenderLabel::setRenderProtocol(std::shared_ptr<AbstrRenderer> renderer)
{
	renderProtocol = renderer;
}

void RenderLabel::rotateCamera(Vec3f rot)
{
	renderProtocol->RotateCamera(rot);
}

void RenderLabel::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Up)
	{
		//RenderServer::getInstance().rotateCamera(renderHandle, Vec3f(0.5f, 0, 0));
		renderProtocol->RotateCamera(Vec3f(0.5f, 0, 0));
	}
}

void RenderLabel::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() & Qt::LeftButton){
		Vec2d pix;
		pix.x = event->x();
		pix.y = event->y();

		//glfwGetCursorPos(m_ClientDisplay, &pix.x, &pix.y);

		Vec2d dir = m_LastMouseClick - pix;
		m_LastMouseClick = pix;

		renderProtocol->RotateCamera(Vec3f(dir.y*0.5, dir.x*0.5, 0));
	}
	else if (event->buttons() & Qt::RightButton) {
		Vec2d pix;
		pix.x = event->x();
		pix.y = event->y();

		//glfwGetCursorPos(m_ClientDisplay, &pix.x, &pix.y);

		Vec2d dir = m_LastMouseClick - pix;
		m_LastMouseClick = pix;

		renderProtocol->MoveCamera(Vec3f(dir.x / 1280.0f, dir.y / -720.0f, 0));
	}
	else
	{
		m_LastMouseClick.x = event->x();
		m_LastMouseClick.y = event->y();
	}
}

void RenderLabel::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		//std::cout << "Right" << std::endl;
	}
	else
	{
		//std::cout << "Left" << std::endl;
	}

	//emit clicked();
}

