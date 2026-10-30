#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include <glm/gtc/matrix_transform.hpp>

#include <gui/view3d/View3D.h>

// Qt code is allowed only inside classes that inherit from QObject
class MainWindow :
	public QMainWindow,
	protected EventDispatcher<IMouseEventListener>,
	protected EventDispatcher<IKeyboardEventListener> {
	
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	// Convert Qt mouse and keyboard events to generic events and dispatch them to the listeners
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	View3D* view3D = nullptr;
	QPoint m_lastCursorPosition;
	std::unordered_map<Qt::MouseButton, KeyID> m_mouseButtonMap;
	std::unordered_map<Qt::Key, KeyID> m_keyboardKeyMap;
};

#endif // MAIN_WINDOW_H