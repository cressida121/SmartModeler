#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtOpenGL/QOpenGLWindow>
#include <QtCore/QTimer>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class MainWindow : public QOpenGLWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	uint32_t m_vertexShader = 0;
	uint32_t m_fragmentShader = 0;
	uint32_t m_shaderProgram = 0;
	uint32_t m_vao = 0;
	uint32_t m_vbo = 0;
	uint32_t m_ebo = 0;
	GLuint m_texture = 0;
	GLuint m_cameraMatrixLocation = 0;
	GLuint m_projectionMatrixLocation = 0;
	glm::mat4x4 m_projectionMatrix = {0};
	Camera *m_pCamera = nullptr;
	QTimer m_timer;
	GLuint m_framebuffer = 0;
	GLuint m_renderTargetTex = 0;
	double m_rps = 0.5; // Full 360 rotations per second
};

#endif // MAIN_WINDOW_H