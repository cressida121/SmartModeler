#include "Camera.h"

void FreeCamera::SetOrientationEuler(glm::vec3 eulerAngles) {
	m_rotation = glm::identity<glm::mat4>();
	m_rotation = glm::rotate(m_rotation, glm::radians(eulerAngles.y), { 0.0f, 1.0f, 0.0f });
	m_rotation = glm::rotate(m_rotation, glm::radians(eulerAngles.x), { 1.0f, 0.0f, 0.0f });
	m_rotation = glm::rotate(m_rotation, glm::radians(eulerAngles.z), { 0.0f, 0.0f, 1.0f });
}

void FreeCamera::TranslateLocal(glm::vec3 delta) {
	glm::vec3 transformedTranslation = m_rotation * glm::vec4(delta, 0.0f);
	m_position += transformedTranslation;
}

glm::mat4 FreeCamera::GetViewTransform() const {
	glm::mat4 translationInv = glm::translate(glm::identity<glm::mat4>(), { -m_position.x, -m_position.y, -m_position.z });
	// Return the inverse of TR -> TR^-1 = R^-1 * T^-1 = R^T * T^-1 since rotation matrix is orthogonal
	return glm::transpose(m_rotation) * translationInv;
}

glm::mat4 FreeCamera::GetTransform() const {
	glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), { m_position.x, m_position.y, m_position.z });
	return translation * m_rotation;
}

void TurntableCamera::SetTarget(glm::vec3 newTarget) {
	m_target = newTarget;
}

void TurntableCamera::SetDistance(float distance) {
	// Prevent negative or zero distance which would cause issues in view transform calculations
	if (distance > 0.01f) {
		m_distance = distance;
	}
}

void TurntableCamera::SetOrientationEuler(glm::vec3 eulerAngles) {
	float azimuth = glm::radians(eulerAngles.y);
	float elevation = glm::radians(eulerAngles.x);

	m_rotation = glm::identity<glm::mat4>();
	m_rotation = glm::rotate(m_rotation, azimuth, { 0.0f, 1.0f, 0.0f });
	m_rotation = glm::rotate(m_rotation, elevation, { 1.0f, 0.0f, 0.0f });
}

void TurntableCamera::TranslateLocal(glm::vec3 delta) {
	glm::vec3 transformedTranslation = m_rotation * glm::vec4(delta, 0.0f);
	m_target += transformedTranslation;
}

glm::mat4 TurntableCamera::GetViewTransform() const {
	glm::mat4 translationInv = glm::identity<glm::mat4>();
	// Move camera back from the target by distance along the local forward axis
	translationInv[3] = -(m_distance * m_rotation[2] + glm::vec4(m_target, 0.0f));
	translationInv[3][3] = 1.0f;
	// Return the inverse of TR -> TR^-1 = R^-1 * T^-1 = R^T * T^-1 since rotation matrix is orthogonal
	return glm::transpose(m_rotation) * translationInv;
}

glm::mat4 TurntableCamera::GetTransform() const {
	glm::mat4 translation = glm::identity<glm::mat4>();
	// Move camera back from the target by distance along the local forward axis
	translation[3] = m_distance * m_rotation[2] + glm::vec4(m_target, 0.0f);
	translation[3][3] = 1.0f;

	return translation * m_rotation;
}