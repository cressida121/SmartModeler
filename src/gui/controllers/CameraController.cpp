#include "CameraController.h"
#include <iostream>

void CameraController::OnMouseMove(int posx, int posy) {
}

void CameraController::OnMouseMoveDelta(int dx, int dy) {

	if (m_lmbPressed) {
		m_rotation.x -= static_cast<float>(dy);
		m_rotation.y -= static_cast<float>(dx);

		m_pCamera.lock()->SetOrientationEuler(m_rotation);
	}

	if (false) {
		m_panDelta.x = static_cast<float>(-dx) * m_panSpeed;
		m_panDelta.y = static_cast<float>(dy) * m_panSpeed;
	}

	if (false) {
		m_zoom = std::max(10.0f, m_zoom + static_cast<float>(dy));
		m_zoom = std::min(1000.0f, m_zoom);
	}

}

void CameraController::OnMouseButtonPress(KeyID keyid) {
	m_lmbPressed = true;
}

void CameraController::OnMouseButtonRelease(KeyID keyid) {
	m_lmbPressed = false;
}

void CameraController::OnMouseWheelScroll(int delta) {
}

void CameraController::SetCamera(std::shared_ptr<Camera> &camera) {
	m_pCamera = camera;
}