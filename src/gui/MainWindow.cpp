#include "MainWindow.h"

#include <QtWidgets/qlayout.h>
#include <QtGui/QMouseEvent>

MainWindow::MainWindow() : QMainWindow()
{
	m_mouseButtonMap = {
		{Qt::MouseButton::LeftButton, KeyID::LMB},
		{Qt::MouseButton::MiddleButton, KeyID::MMB},
		{Qt::MouseButton::RightButton, KeyID::RMB}
	};

	m_keyboardKeyMap = {
		{Qt::Key::Key_Shift, KeyID::SHIFT},
		{Qt::Key::Key_Control, KeyID::CONTROL},
		{Qt::Key::Key_Alt, KeyID::ALT}
	};

	view3D = new View3D();
	setCentralWidget(view3D);

	//auto mouseListenerPtr = std::dynamic_pointer_cast<IMouseEventListener>(m_view3DController);
	//EventDispatcher<IMouseEventListener>::AddEventListener(m_view3DController);

}

MainWindow::~MainWindow()
{
	delete view3D;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {

	if (m_mouseButtonMap.count(event->button()) == 0) {
		return;
	}

	KeyID keyid = m_mouseButtonMap[event->button()];
	EventDispatcher<IMouseEventListener>::DispatchEvent([keyid](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseButtonPress(keyid); });
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {

	if (m_mouseButtonMap.count(event->button()) == 0) {
		return;
	}

	KeyID keyid = m_mouseButtonMap[event->button()];
	EventDispatcher<IMouseEventListener>::DispatchEvent([keyid](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseButtonRelease(keyid); });
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {

	QPoint cursorPosition = event->pos();

	EventDispatcher<IMouseEventListener>::DispatchEvent([cursorPosition](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseMove(cursorPosition.x(), cursorPosition.y()); });

	const QPoint delta = cursorPosition - m_lastCursorPosition;

	m_lastCursorPosition = cursorPosition;

	EventDispatcher<IMouseEventListener>::DispatchEvent([delta](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseMoveDelta(delta.x(), delta.y()); });
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

	if (m_keyboardKeyMap.count(Qt::Key(event->key())) == 0) {
		return;
	}

	KeyID keyid = m_keyboardKeyMap[Qt::Key(event->key())];
	EventDispatcher<IKeyboardEventListener>::DispatchEvent([keyid](std::shared_ptr<IKeyboardEventListener> listener) {
		listener->OnKeyPress(keyid); });
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {

	if (m_keyboardKeyMap.count(Qt::Key(event->key())) == 0) {
		return;
	}

	KeyID keyid = m_keyboardKeyMap[Qt::Key(event->key())];
	EventDispatcher<IKeyboardEventListener>::DispatchEvent([keyid](std::shared_ptr<IKeyboardEventListener> listener) {
		listener->OnKeyRelease(keyid); });
}