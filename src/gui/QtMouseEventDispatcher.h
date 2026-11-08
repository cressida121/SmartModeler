#ifndef QT_MOUSE_EVENT_DISPATCHER_H
#define QT_MOUSE_EVENT_DISPATCHER_H

#include <core/EventDispatcher.h>
#include <gui/IUserInputListener.h>

#include <QtGui/QMouseEvent>

class QtMouseEventDispatcher : public EventDispatcher<IMouseEventListener> {

public:
	QtMouseEventDispatcher() {
		m_mouseButtonMap[Qt::LeftButton] = KeyID::LMB;
		m_mouseButtonMap[Qt::RightButton] = KeyID::RMB;
		m_mouseButtonMap[Qt::MiddleButton] = KeyID::MMB;
	}

	void QtMousePressEvent(QMouseEvent *event);
	void QtMouseReleaseEvent(QMouseEvent *event);
	void QtMouseMoveEvent(QMouseEvent *event);

private:
	QPoint m_lastCursorPosition;
	std::unordered_map<Qt::MouseButton, KeyID> m_mouseButtonMap;
};

#endif // !QT_MOUSE_EVENT_DISPATCHER_H
