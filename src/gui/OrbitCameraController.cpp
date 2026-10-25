#include "OrbitCameraController.h"
#include <iostream>

void OrbitCameraController::ProcessInput(const double frameTimeMS) {

	if (!m_orbitInputBuffer.size() || m_pCamera.expired()) {
		return;
	}

	if (m_counter == 0u) {

		m_framesCount = static_cast<uint32_t>(m_inputLagMS / frameTimeMS);
		m_anglePerFrame = m_orbitInputBuffer.front();
		m_orbitInputBuffer.pop();

		if (m_framesCount > 0u) {
			m_anglePerFrame.first /= static_cast<double>(m_framesCount);
			m_anglePerFrame.second /= static_cast<double>(m_framesCount);
		}
	}

	m_rotation.x -= static_cast<float>(m_anglePerFrame.second);
	m_rotation.y -= static_cast<float>(m_anglePerFrame.first);
	m_pCamera.lock()->SetOrientationEuler(m_rotation);

	if (m_counter < m_framesCount) {
		m_counter++;
		return;
	}

	m_counter = 0u;

}

void OrbitCameraController::OnMouseMove(int posx, int posy) {
}

void OrbitCameraController::OnMouseMoveDelta(int dx, int dy) {
	if (KeysDown(m_orbitKeys) && m_orbitInputBuffer.size() < m_inputBufferSize) {
		m_orbitInputBuffer.push({ dx, dy });
	}
}

void OrbitCameraController::OnMouseButtonPress(KeyID keyid) {
	m_keysPressed[keyid] = true;
}

void OrbitCameraController::OnMouseButtonRelease(KeyID keyid) {
	m_keysPressed[keyid] = false;
}

void OrbitCameraController::OnMouseWheelScroll(int delta) {
}

void OrbitCameraController::OnKeyPress(KeyID keyid) {
	m_keysPressed[keyid] = true;
}

void OrbitCameraController::OnKeyRelease(KeyID keyid) {
	m_keysPressed[keyid] = false;
}

bool OrbitCameraController::KeysDown(const std::vector<KeyID> keys) const {
	for (auto &keyID : keys) {
		if (!m_keysPressed[keyID]) {
			return false;
		}
	}
	return true;
}
