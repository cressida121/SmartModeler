#ifndef ORBIT_CAMERA_CONTROLLER_H
#define ORBIT_CAMERA_CONTROLLER_H

#include <core/Camera.h>
#include <gui/IUserInputListener.h>

#include <memory>
#include <array>
#include <queue>

class CameraController : 
	public IMouseEventListener{

public:
	CameraController(std::shared_ptr<Camera> &camera) : m_pCamera(camera) {}
	
	void SetCamera(std::shared_ptr<Camera> &camera);

private:

	// Inherited via IMouseEventListener
	void OnMouseMove(int posx, int posy) override;

	void OnMouseMoveDelta(int dx, int dy) override;

	void OnMouseButtonPress(KeyID keyid) override;

	void OnMouseButtonRelease(KeyID keyid) override;

	void OnMouseWheelScroll(int delta) override;


	// Style - group logically realted fields

	std::weak_ptr<Camera> m_pCamera;
	float m_panSpeed = 0.1f;
	glm::vec3 m_rotation{0.};
	glm::vec3 m_panDelta{0.};
	float m_zoom = 100.0f;
	bool m_lmbPressed = false;
};

#endif // !ORBIT_CAMERA_CONTROLLER_H