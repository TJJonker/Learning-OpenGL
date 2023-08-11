#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
private:
	glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
	glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	bool m_FirstMouse = true;
	float m_Fov = 45.0f;

	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;

	glm::vec2 m_LastMousePosition;

	const float m_Sensitivity = 0.1f;
	const float m_CameraSpeed = 2.5f;
public:
	glm::mat4 GetView() const;
	float GetFOV() const;
	glm::vec3 GetPosition() const;

	void ScrollCamera(float yOffset);
	void LookCamera(const glm::vec2& position);
	void MoveCamera(glm::vec2 movement);
};

