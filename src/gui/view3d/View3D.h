#ifndef VIEW3D_H
#define VIEW3D_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <glm/gtc/matrix_transform.hpp>

#include <gui/controllers/CameraController.h>
#include <gui/QtMouseEventDispatcher.h>

// Qt code is allowed only inside classes that inherit from QObject

class View3D: public QOpenGLWidget, public QtMouseEventDispatcher {

	Q_OBJECT

public:
	View3D(QWidget *parent, std::shared_ptr<Camera> camera);
	~View3D();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void BindCamera(std::shared_ptr<Camera> camera);

	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

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
	std::weak_ptr<Camera> m_pCamera;
	std::chrono::high_resolution_clock::time_point m_lastFrameTime;
	double m_frameTimeMs = 0.0; // Time taken to render the last frame in milliseconds

	bool m_isMousePressed = false;

};

#endif // VIEW3D_H