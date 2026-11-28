#include "FLlib.h"
#include "PerspectiveCamera.h"

void ProcessEvents(FLlib::PerspectiveCamera& camera);

int main() {
	FLlib::Log::init();
	auto window = FLlib::Window::Create({1920,1080, "Window"});
	FLlib::Renderer::Init();

	auto& running = FLlib::Globals::AppShouldRun;
	auto& camera = FLlib::Globals::Camera = FLlib::PerspectiveCamera(45.0f, (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);
	camera.SetPosition({ 0.0f, 0.0f, 2.0f });


	while (running) {
		ProcessEvents(camera);

		if (FLlib::Input::OnKeyPressed(FLlib::Key::ESCAPE)) {
			running = false;
		}

		FLlib::Globals::timeStep.Update();

		camera.OnUpdate();

		static FLlib::RectColor color;
		color.Top = FLlib::Color::Red;
		color.Bottom = FLlib::Color::Green;
		color.Front = FLlib::Color::Blue;
		color.Back = FLlib::Color::Cyan;
		color.Right = FLlib::Color::Magenta;
		color.Left = FLlib::Color::Lime;

		FLlib::Renderer::BeginRender();
		FLlib::Renderer::RenderRect({0.0f,0.0f,0.0f}, {1.0f,1.0f,3.0f}, color);
		FLlib::Renderer::RenderSphere({ 3.0f,0.0f,0.0f }, 2.0f, FLlib::Color::Black);
		FLlib::Renderer::RenderLine({ 0.0f,0.0f,0.0f }, { 1.0f,2.0f,3.0f }, FLlib::Color::Magenta);
		FLlib::Renderer::EndRender();

		FLlib::Globals::Window->OnUpdate();
	}
}

void ProcessEvents(FLlib::PerspectiveCamera& camera) {
	while (auto event = FLlib::Events::Poll()) {
		std::visit(FLlib::Overloaded{
			[](const FLlib::WindowResizeEvent& e) { LOG_INFO("Window resized to {}x{}", e.Width, e.Height); },
			[&](const FLlib::MouseScrollEvent& e) { camera.SetFov(glm::clamp(camera.GetFov() - e.YOffset * 2.0f, 10.0f, 90.0f)); },
			[](const auto&) {}
			}, *event);
	}
}