#ifndef ORBIT_CAMERA_CONTROLLER_H
#define ORBIT_CAMERA_CONTROLLER_H

#include <core/Camera.h>
#include <gui/IUserInputListener.h>

#include <memory>
#include <array>
#include <queue>

class OrbitCameraController : 
	public IMouseEventListener,
	public IKeyboardEventListener {

public:
	OrbitCameraController(std::shared_ptr<OrbitCamera> &camera) : m_pCamera(camera) {}
	
	void SetCamera(std::shared_ptr<OrbitCamera> &camera) {
		m_pCamera = camera;
	}

	void ProcessInput(const double frameTimeMS); 

private:

	bool KeysDown(const std::vector<KeyID> keys) const;

	// Inherited via IMouseEventListener
	void OnMouseMove(int posx, int posy) override;

	void OnMouseMoveDelta(int dx, int dy) override;

	void OnMouseButtonPress(KeyID keyid) override;

	void OnMouseButtonRelease(KeyID keyid) override;

	void OnMouseWheelScroll(int delta) override;

	// Inherited via IKeyboardEventListener
	void OnKeyPress(KeyID keyid) override;

	void OnKeyRelease(KeyID keyid) override;


	// Style - group logically realted fields

	std::weak_ptr<OrbitCamera> m_pCamera;

	std::array<bool, 7> m_keysPressed = { false, false, false, false, false, false, false };
	std::vector<KeyID> m_orbitKeys = {KeyID::LMB, KeyID::ALT};
	std::vector<KeyID> m_panKeys = { KeyID::LMB, KeyID::SHIFT };
	std::vector<KeyID> m_zoomKeys = { KeyID::LMB, KeyID::CONTROL };

	std::queue<std::pair<int, int>> m_orbitInputBuffer;
	uint32_t m_inputBufferSize = 10u;

	uint32_t m_inputLagMS = 0u;
	uint32_t m_counter = 0u;
	uint32_t m_framesCount = 0u;

	std::pair<double, double> m_anglePerFrame;
	float m_panSpeed = 0.1f;
	glm::vec3 m_rotation{0.};
	glm::vec3 m_panDelta{0.};
	float m_zoom = 100.0f;
};

#endif // !ORBIT_CAMERA_CONTROLLER_H