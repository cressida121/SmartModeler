#ifndef VIEW3D_H
#define VIEW3D_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtCore/QTimer>
#include <glm/gtc/matrix_transform.hpp>

#include <gui/IUserInputListener.h>
#include <gui/controllers/OrbitCameraController.h>
#include <core/EventDispatcher.h>

// Qt code is allowed only inside classes that inherit from QObject
class View3D:
	public QOpenGLWidget,
	protected EventDispatcher<IMouseEventListener>,
	protected EventDispatcher<IKeyboardEventListener> {

	Q_OBJECT

public:
	View3D();
	~View3D();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	// Convert Qt mouse and keyboard events to generic events and dispatch them to the listeners
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

private:
	GLuint m_vertexShader = 0;
	GLuint m_fragmentShader = 0;
	GLuint m_shaderProgram = 0;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	GLuint m_texture = 0;
	GLuint m_cameraMatrixLocation = 0;
	GLuint m_projectionMatrixLocation = 0;
	GLuint m_framebuffer = 0;
	GLuint m_renderTargetTex = 0;

	glm::mat4x4 m_projectionMatrix = {0};
	std::shared_ptr<Camera> m_pCamera;
	std::chrono::high_resolution_clock::time_point m_lastFrameTime;
	double m_frameTimeMs = 0.0; // Time taken to render the last frame in milliseconds

	std::unordered_map<Qt::MouseButton, KeyID> m_mouseButtonMap;
	std::unordered_map<Qt::Key, KeyID> m_keyboardKeyMap;
	std::shared_ptr<OrbitCameraController> m_orbitCameraController;
	QPoint m_lastCursorPosition;
};

#endif // VIEW3D_H