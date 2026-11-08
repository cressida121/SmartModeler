#include <gui/QtMouseEventDispatcher.h>

void QtMouseEventDispatcher::QtMousePressEvent(QMouseEvent *event) {

	if (m_mouseButtonMap.count(event->button()) == 0) {
		return;
	}

	KeyID keyid = m_mouseButtonMap[event->button()];
	EventDispatcher<IMouseEventListener>::DispatchEvent([keyid](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseButtonPress(keyid); });
}

void QtMouseEventDispatcher::QtMouseReleaseEvent(QMouseEvent *event) {

	if (m_mouseButtonMap.count(event->button()) == 0) {
		return;
	}

	KeyID keyid = m_mouseButtonMap[event->button()];
	EventDispatcher<IMouseEventListener>::DispatchEvent([keyid](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseButtonRelease(keyid); });
}

void QtMouseEventDispatcher::QtMouseMoveEvent(QMouseEvent *event) {

	QPoint cursorPosition = event->pos();

	EventDispatcher<IMouseEventListener>::DispatchEvent([cursorPosition](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseMove(cursorPosition.x(), cursorPosition.y()); });

	const QPoint delta = cursorPosition - m_lastCursorPosition;

	m_lastCursorPosition = cursorPosition;

	EventDispatcher<IMouseEventListener>::DispatchEvent([delta](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseMoveDelta(delta.x(), delta.y()); });
}
