#ifndef ORBIT_CAMERA_CONTROLLER_H
#define ORBIT_CAMERA_CONTROLLER_H

#include <core/Camera.h>
#include "IUserInputListener.h"

#include <memory>
#include <array>

class OrbitCameraController : 
	public IMouseEventListener,
	public IKeyboardEventListener {

public:
	OrbitCameraController(std::shared_ptr<OrbitCamera> &camera) : m_pCamera(camera) {}
	
	void SetCamera(std::shared_ptr<OrbitCamera> &camera) {
		m_pCamera = camera;
	}

private:

	// Inherited via IMouseEventListener
	void OnMouseMove(int posx, int posy) override;

	void OnMouseMoveDelta(int dx, int dy) override;

	void OnMouseButtonPress(KeyID keyid) override;

	void OnMouseButtonRelease(KeyID keyid) override;

	void OnMouseWheelScroll(int delta) override;


	// Inherited via IKeyboardEventListener
	void OnKeyPress(KeyID keyid) override;

	void OnKeyRelease(KeyID keyid) override;


	std::weak_ptr<OrbitCamera> m_pCamera;
	std::array<bool, 7> m_keysPressed = { false, false, false, false, false, false, false };
};

#endif // !ORBIT_CAMERA_CONTROLLER_H