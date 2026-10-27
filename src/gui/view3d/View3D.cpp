#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include "View3D.h"
#include <core/Camera.h>

#include <glm/gtc/type_ptr.hpp>

std::vector<char> LoadCompiledShaderBytes(std::string filePath) {

	std::vector<char> bytes;
	std::ifstream shaderFile(filePath, std::ios::ate | std::ios::binary);

	if (shaderFile.fail()) {
		return bytes;
	}

	size_t fileSize = shaderFile.tellg();

	bytes.resize(fileSize);

	shaderFile.seekg(0);
	shaderFile.read(bytes.data(), fileSize);
	shaderFile.close();

	return bytes;
}

extern std::vector<uint8_t> GetImagePixels(std::wstring file, uint32_t &width, uint32_t &height, uint32_t &pixelByteSize);


View3D::View3D() : QOpenGLWidget() {

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

	QSurfaceFormat format;
	format.setSamples(8);
	setFormat(format);
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	m_pCamera = std::make_shared<OrbitCamera>();

	auto orbitCameraPtr = std::dynamic_pointer_cast<OrbitCamera>(m_pCamera);
	m_orbitCameraController = std::make_shared<OrbitCameraController>(orbitCameraPtr);
	
	auto mouseListenerPtr = std::dynamic_pointer_cast<IMouseEventListener>(m_orbitCameraController);
	EventDispatcher<IMouseEventListener>::AddEventListener(mouseListenerPtr);

	auto keyboardListenerPtr = std::dynamic_pointer_cast<IKeyboardEventListener>(m_orbitCameraController);
	EventDispatcher<IKeyboardEventListener>::AddEventListener(keyboardListenerPtr);

	connect(this, QOverload<>::of(&View3D::frameSwapped), this, QOverload<>::of(&View3D::update));

}

View3D::~View3D() {

	glDeleteProgram(m_shaderProgram);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteFramebuffers(1, &m_framebuffer);
	glDeleteTextures(1, &m_texture);

}

void View3D::initializeGL() {

	std::filesystem::path compiledShaderPath(_COMPILED_SHADERS_DIR);
	compiledShaderPath.append("VertexShader.gl");

	std::vector<char> vsBytes = LoadCompiledShaderBytes(compiledShaderPath.string());

	compiledShaderPath.replace_filename("FragmentShader.gl");

	std::vector<char> fsBytes = LoadCompiledShaderBytes(compiledShaderPath.string());

	gladLoadGL();

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderBinary(1, &m_vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vsBytes.data(), static_cast<GLsizei>(vsBytes.size()));
	glShaderBinary(1, &m_fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fsBytes.data(), static_cast<GLsizei>(fsBytes.size()));
	
	glSpecializeShader(m_vertexShader, "main", 0, nullptr, nullptr);
	glSpecializeShader(m_fragmentShader, "main", 0, nullptr, nullptr);

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);
	glLinkProgram(m_shaderProgram);

	GLint status;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);

	if (!status) {
		return;
	}

	m_cameraMatrixLocation = glGetUniformLocation(m_shaderProgram, "cameraMatrix");
	m_projectionMatrixLocation = glGetUniformLocation(m_shaderProgram, "projectionMatrix");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	float vertices[] = {
	-15.f, -15.f, 0.0f,
	 1.0f, 0.7f, 0.7f,
	 0.0f, 1.0f,

	 15.f, -15.f, 0.0f,
	 0.7f, 1.0f, 0.7f,
	 1.0f, 1.0f,

	-15.f, 15.f, 0.0f,
	 0.7f, 0.7f, 1.0f,
	 0.0f, 0.0f,

	 15.f, 15.f, 0.0f,
	 0.7f, 1.0f, 1.0f,
	 1.0f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	GLuint indices[] = { 0, 1, 2, 2, 1, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	std::filesystem::path imgPath(_RESOURCES_DIR);
	imgPath.append("brick.png");

	uint32_t imgWidth = 0;
	uint32_t imgHeight = 0;
	uint32_t size = 0;
	std::vector<uint8_t> imgData = GetImagePixels(imgPath.wstring(), imgWidth, imgHeight, size);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, imgWidth, imgHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, imgData.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 2.);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1u, &m_renderTargetTex);
	glBindTexture(GL_TEXTURE_2D, m_renderTargetTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0u);

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTargetTex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_MULTISAMPLE);
}


void View3D::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);

	double aspectRatio = static_cast<double>(width()) / static_cast<double>(height());
	m_projectionMatrix = glm::perspective(glm::radians(45.0), aspectRatio, 0.1, 500.);
}

void View3D::paintGL() {

	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_frameTimeMs = std::chrono::duration<double, std::milli>(currentTime - m_lastFrameTime).count();
	m_lastFrameTime = currentTime;

	m_orbitCameraController->ProcessInput(m_frameTimeMs);

	glm::mat4 cameraMatrix = glm::identity<glm::mat4>();

	if(m_pCamera) {
		cameraMatrix = m_pCamera->GetViewTransform();
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glClearColor(0.3f, 0.3f, 0.35f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(m_cameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void View3D::mousePressEvent(QMouseEvent *event) {

	if(m_mouseButtonMap.count(event->button()) == 0) {
		return;
	}

	KeyID keyid = m_mouseButtonMap[event->button()];
	EventDispatcher<IMouseEventListener>::DispatchEvent([keyid](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseButtonPress(keyid); });
}

void View3D::mouseReleaseEvent(QMouseEvent *event) {

	if (m_mouseButtonMap.count(event->button()) == 0) {
		return;
	}

	KeyID keyid = m_mouseButtonMap[event->button()];
	EventDispatcher<IMouseEventListener>::DispatchEvent([keyid](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseButtonRelease(keyid); });
}

void View3D::mouseMoveEvent(QMouseEvent *event) {

	QPoint cursorPosition = event->pos();
	
	EventDispatcher<IMouseEventListener>::DispatchEvent([cursorPosition](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseMove(cursorPosition.x(), cursorPosition.y()); });

	const QPoint delta = cursorPosition - m_lastCursorPosition;

	m_lastCursorPosition = cursorPosition;

	EventDispatcher<IMouseEventListener>::DispatchEvent([delta](std::shared_ptr<IMouseEventListener> listener) {
		listener->OnMouseMoveDelta(delta.x(), delta.y()); });
}

void View3D::keyPressEvent(QKeyEvent *event) {

	if(m_keyboardKeyMap.count(Qt::Key(event->key())) == 0) {
		return;
	}

	KeyID keyid = m_keyboardKeyMap[Qt::Key(event->key())];
	EventDispatcher<IKeyboardEventListener>::DispatchEvent([keyid](std::shared_ptr<IKeyboardEventListener> listener) {
		listener->OnKeyPress(keyid); });
}

void View3D::keyReleaseEvent(QKeyEvent *event) {

	if (m_keyboardKeyMap.count(Qt::Key(event->key())) == 0) {
		return;
	}

	KeyID keyid = m_keyboardKeyMap[Qt::Key(event->key())];
	EventDispatcher<IKeyboardEventListener>::DispatchEvent([keyid](std::shared_ptr<IKeyboardEventListener> listener) {
		listener->OnKeyRelease(keyid); });
}
