#pragma once

#include "Globals.h"
#include "Input.h"
#include "Window.h"
#include "PerspectiveCamera.h"

namespace FLlib {
	// WASD: move horizontally relative to where the camera faces, E / Q: up / down, hold right mouse button: look around
	inline void CameraController(FLlib::PerspectiveCamera& camera) {
		using FLlib::Input;
		using FLlib::Key;

		const float moveSpeed = 3.0f;
		const float mouseSensitivity = 0.1f;

		glm::vec3 position = camera.GetPosition();

		// Forward/right from yaw only, so looking up or down doesn't change height
		const float yawRad = glm::radians(camera.GetYaw());
		const glm::vec3 forward = { cos(yawRad), 0.0f, sin(yawRad) };
		const glm::vec3 right = { -forward.z, 0.0f, forward.x };
		const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

		auto dt = Globals::timeStep.GetSeconds();

		if (Input::IsKeyPressed(Key::W)) position += forward * moveSpeed * dt;
		if (Input::IsKeyPressed(Key::S)) position -= forward * moveSpeed * dt;
		if (Input::IsKeyPressed(Key::D)) position += right * moveSpeed * dt;
		if (Input::IsKeyPressed(Key::A)) position -= right * moveSpeed * dt;
		if (Input::IsKeyPressed(Key::E)) position += up * moveSpeed * dt;
		if (Input::IsKeyPressed(Key::Q)) position -= up * moveSpeed * dt;

		float pitch = camera.GetPitch();
		float yaw = camera.GetYaw();

		const bool looking = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);
		if (looking != Globals::Window->IsCursorLocked())
			Globals::Window->SetCursorLocked(looking);
		else if (looking) {
			const glm::vec2 delta = Input::GetMouseDelta();
			yaw += delta.x * mouseSensitivity;
			pitch -= delta.y * mouseSensitivity;
		}

		camera.SetPosition(position);
		camera.SetRotation(pitch, yaw);
	}
}