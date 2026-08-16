#include "OrbitCameraController.h"
#include <iostream>

void OrbitCameraController::OnMouseMove(int posx, int posy) {
	if(m_pCamera.expired()) {
		return;
	}

	if (m_keysPressed[KeyID::SHIFT]) {
		std::cout<<"Shift down"<<std::endl;
	}
}

void OrbitCameraController::OnMouseMoveDelta(int dx, int dy) {
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
