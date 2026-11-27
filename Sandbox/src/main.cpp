#include "FLlib.h"
#include "PerspectiveCamera.h"

// WASD: move horizontally relative to where the camera faces, E / Q: up / down, arrow keys: look around
static void UpdateCameraController(FLlib::PerspectiveCamera& camera, float dt) {
	using FLlib::Input;
	using FLlib::Key;

	const float moveSpeed = 3.0f;
	const float lookSpeed = 90.0f;

	glm::vec3 position = camera.GetPosition();

	// Forward/right from yaw only, so looking up or down doesn't change height
	const float yawRad = glm::radians(camera.GetYaw());
	const glm::vec3 forward = { cos(yawRad), 0.0f, sin(yawRad) };
	const glm::vec3 right = { -forward.z, 0.0f, forward.x };
	const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

	if (Input::IsKeyPressed(Key::W)) position += forward * moveSpeed * dt;
	if (Input::IsKeyPressed(Key::S)) position -= forward * moveSpeed * dt;
	if (Input::IsKeyPressed(Key::D)) position += right * moveSpeed * dt;
	if (Input::IsKeyPressed(Key::A)) position -= right * moveSpeed * dt;
	if (Input::IsKeyPressed(Key::E)) position += up * moveSpeed * dt;
	if (Input::IsKeyPressed(Key::Q)) position -= up * moveSpeed * dt;

	float pitch = camera.GetPitch();
	float yaw = camera.GetYaw();

	if (Input::IsKeyPressed(Key::UP)) pitch += lookSpeed * dt;
	if (Input::IsKeyPressed(Key::DOWN)) pitch -= lookSpeed * dt;
	if (Input::IsKeyPressed(Key::RIGHT)) yaw += lookSpeed * dt;
	if (Input::IsKeyPressed(Key::LEFT)) yaw -= lookSpeed * dt;

	camera.SetPosition(position);
	camera.SetRotation(pitch, yaw);
}

int main() {
	FLlib::Log::init();
	FLlib::Window::Create({1920,1080, "Window"});
	FLlib::Renderer::Init();

	auto& running = FLlib::Globals::AppShouldRun;
	auto& camera = FLlib::Globals::Camera = FLlib::PerspectiveCamera(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
	camera.SetPosition({ 0.0f, 0.0f, 2.0f });


	while (running) {
		if (FLlib::Input::OnKeyPressed(FLlib::Key::ESCAPE)) {
			running = false;
		}

		FLlib::Globals::timeStep.Update();

		UpdateCameraController(camera, FLlib::Globals::timeStep.GetSeconds());

		static FLlib::RectColor color;
		color.Top    = FLlib::Color::Red;
		color.Bottom = FLlib::Color::Green;
		color.Front  = FLlib::Color::Blue;
		color.Back   = FLlib::Color::Cyan;
		color.Right  = FLlib::Color::Magenta;
		color.Left   = FLlib::Color::Lime;

		FLlib::Renderer::BeginRender();
		FLlib::Renderer::RenderRect({0.0f,0.0f,0.0f}, {1.0f,1.0f,3.0f}, color);
		FLlib::Renderer::RenderSphere({ 1.0f,0.0f,0.0f }, 2.0f, FLlib::Color::Black);
		FLlib::Renderer::RenderLine({ 0.0f,0.0f,0.0f }, { 1.0f,2.0f,3.0f }, FLlib::Color::Magenta);
		FLlib::Renderer::EndRender();

		FLlib::Globals::Window->OnUpdate();
	}
}
