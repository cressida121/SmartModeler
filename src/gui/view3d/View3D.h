#ifndef VIEW3D_H
#define VIEW3D_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtCore/QTimer>
#include <glm/gtc/matrix_transform.hpp>

#include <gui/IUserInputListener.h>
#include <gui/controllers/OrbitCameraController.h>
#include <core/EventDispatcher.h>

// Qt code is allowed only inside classes that inherit from QObject

class View3D: public QOpenGLWidget{

	Q_OBJECT

public:
	View3D();
	~View3D();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	//void Rotate(dx,dy,dz)
	//void Pan(dx,dy) - ignored for free camera
	//void Zoom(delta) - ignored for free camera
	//void Translate(dx,dy,dz) - only for free camera

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
	
};


class View3DMouseController : public IMouseEventListener/*, AppActionListener*/ {

public:

	View3DMouseController(View3D* view) : m_view(view) {}

	// Inherited via IMouseEventListener
	void OnMouseMove(int posx, int posy) override;
	void OnMouseMoveDelta(int dx, int dy) override;
	void OnMouseButtonPress(KeyID keyid) override;
	void OnMouseButtonRelease(KeyID keyid) override;
	void OnMouseWheelScroll(int delta) override;

private:
	View3D* m_view = nullptr;

};

#endif // VIEW3D_H