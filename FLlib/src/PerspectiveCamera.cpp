#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace FLlib
{
	static constexpr glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		RecalculateProjection();
		RecalculateView();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_Fov = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		RecalculateProjection();
	}

	void PerspectiveCamera::SetFov(float fov)
	{
		m_Fov = fov;
		RecalculateProjection();
	}

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		RecalculateProjection();
	}

	void PerspectiveCamera::SetClipPlanes(float nearClip, float farClip)
	{
		m_NearClip = nearClip;
		m_FarClip = farClip;
		RecalculateProjection();
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateView();
	}

	void PerspectiveCamera::SetRotation(float pitch, float yaw)
	{
		m_Pitch = glm::clamp(pitch, -89.0f, 89.0f);
		m_Yaw = yaw;\
		RecalculateView();
	}

	void PerspectiveCamera::RecalculateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjection = m_Projection * m_View;
	}

	void PerspectiveCamera::RecalculateView()
	{
		const float pitch = glm::radians(m_Pitch);
		const float yaw = glm::radians(m_Yaw);

		m_Forward = glm::normalize(glm::vec3(
			cos(yaw) * cos(pitch),
			sin(pitch),
			sin(yaw) * cos(pitch)));
		m_Right = glm::normalize(glm::cross(m_Forward, WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

		m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
		m_ViewProjection = m_Projection * m_View;
	}
}
