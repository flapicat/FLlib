#pragma once
#include <glm/glm.hpp>

namespace FLlib
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 1000.0f);

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);
		void SetFov(float fov);
		void SetAspectRatio(float aspectRatio);
		void SetClipPlanes(float nearClip, float farClip);

		void SetPosition(const glm::vec3& position);
		void SetRotation(float pitch, float yaw);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
		float GetFov() const { return m_Fov; }
		float GetAspectRatio() const { return m_AspectRatio; }

		const glm::vec3& GetForward() const { return m_Forward; }
		const glm::vec3& GetRight() const { return m_Right; }
		const glm::vec3& GetUp() const { return m_Up; }

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetViewProjection() const { return m_ViewProjection; }
	private:
		void RecalculateProjection();
		void RecalculateView();
	private:
		float m_Fov;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Pitch = 0.0f;
		float m_Yaw = -90.0f;

		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_ViewProjection = glm::mat4(1.0f);
	};
}
