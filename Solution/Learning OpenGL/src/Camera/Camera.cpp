#include "Camera.h"
#include <algorithm>
#include "../Utils/Time.h"

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}

float Camera::GetFOV() const
{
	return m_Fov;
}

glm::vec3 Camera::GetPosition() const
{
	return m_CameraPos;
}

glm::vec3 Camera::GetCameraFront() const
{
	return m_CameraFront;
}

void Camera::ScrollCamera(float yOffset)
{
	m_Fov -= yOffset;
	m_Fov = std::max(1.0f, std::min(m_Fov, 45.0f));
}

void Camera::LookCamera(const glm::vec2& position)
{
	if (m_FirstMouse) {
		m_LastMousePosition = position;
		m_FirstMouse = false;
	}

	glm::vec2 offset = { position.x - m_LastMousePosition.x, m_LastMousePosition.y - position.y };
	m_LastMousePosition = position;

	offset *= m_Sensitivity;

	m_Yaw += offset.x;
	m_Pitch += offset.y;

	m_Pitch = std::max(-89.0f, std::min(m_Pitch, 89.0f));
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront = glm::normalize(direction);
}

void Camera::MoveCamera(glm::vec2 movement)
{
	const float cameraMovementSpeed = m_CameraSpeed * Time::DeltaTime();

	m_CameraPos += cameraMovementSpeed * m_CameraFront * movement.y;
	m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraMovementSpeed * movement.x;
}
