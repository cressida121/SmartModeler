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

	glm::vec3 m_cameraRotation {0.0f, 0.0f, 0.0f};
	glm::mat4x4 m_projectionMatrix = {0};
	Camera *m_pCamera = nullptr;
	std::chrono::high_resolution_clock::time_point m_lastFrameTime;
	double m_rps = 0.5; // Full 360 rotations per second
	double m_frameTimeMs = 0.0; // Time taken to render the last frame in milliseconds
};

#endif // MAIN_WINDOW_H