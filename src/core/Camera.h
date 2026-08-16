#ifndef CAMERA_H
#define CAMERA_H

#include "SceneNode.h"

class Camera : public SceneNode {
public:
	Camera(SceneNode *parent = nullptr, std::string name = "") : SceneNode(parent, name) {};
	virtual ~Camera() = default;
	virtual glm::mat4 GetViewTransform() const = 0;

	// TranslateLocal the camera by the given delta in local (view) space 
	virtual void TranslateLocal(glm::vec3 delta) = 0;

	// Set the camera orientation by the given Euler angles (in degrees)
	virtual void SetOrientationEuler(glm::vec3 eulerAngles) = 0;

protected:
	glm::mat4 m_rotation = glm::identity<glm::mat4>();
};

class FreeCamera : public Camera {
public:
	// Inherited from SceneNode
	glm::mat4 GetTransform() const override;

	// Inherited from Camera
	glm::mat4 GetViewTransform() const override;
	void TranslateLocal(glm::vec3 delta) override;
	void SetOrientationEuler(glm::vec3 eulerAngles) override;

	FreeCamera() : Camera(nullptr, "FreeCamera") {};
	void SetPosition(glm::vec3 position) { m_position = position; }

private:
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
};

class OrbitCamera : public Camera {
public:
	// Inherited from SceneNode
	glm::mat4 GetTransform() const override;

	// Inherited from Camera
	glm::mat4 GetViewTransform() const override;
	void SetOrientationEuler(glm::vec3 eulerAngles) override;
	void TranslateLocal(glm::vec3 delta) override;

	OrbitCamera() : Camera(nullptr, "OrbitCamera") {};
	void SetTarget(glm::vec3 newTarget);
	void SetDistance(float distance);

private:
	float m_distance = 100.0f; // Distance between an eye position and the target point
	glm::vec3 m_target = { 0.0f, 0.0f, 0.0f };
};

#endif // CAMERA_H