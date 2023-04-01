#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Camera
{
public:
	Camera(float verticalFOV, float nearClip, float farClip);

	bool OnUpdate(float ts);
	void OnResize(uint32_t width, uint32_t height);

	const glm::mat4& GetProjection() const { return m_Projection; }
	const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetInverseView() const { return m_InverseView; }

	const glm::vec3& GetPosition() const { return m_Position; }

    const glm::vec3 GetRight() const {
        glm::mat4 Right = glm::rotate(m_View, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        return glm::vec3(Right[0][0], Right[1][0], Right[2][0]);
    }
    const glm::vec3 GetUp() const {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
	const glm::vec3& GetDirection() const { return m_ForwardDirection; }

    void GetUI();

	const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }

	float GetRotationSpeed();

private:
	void RecalculateProjection();
	void RecalculateView();
	void RecalculateRayDirections();

    glm::mat4 m_Projection{ 1.0f };
	glm::mat4 m_View{ 1.0f };
	glm::mat4 m_InverseProjection{ 1.0f };
	glm::mat4 m_InverseView{ 1.0f };

    float m_PreviousFOV = 45.0f;
    float m_PreviousNearClip = 0.1f;
    float m_PreviousFarClip = 100.0f;

public:
	float m_VerticalFOV = 45.0f;
	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;
    float m_BlurStrength = 1.0f;
    float m_JitterStrength = 4.0f;

    glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_ForwardDirection{0.0f, 0.0f, 0.0f};

private:
	// Cached ray directions
	std::vector<glm::vec3> m_RayDirections;

	glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};