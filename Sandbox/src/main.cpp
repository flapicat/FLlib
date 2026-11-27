#include "FLlib.h"

int main() {
	FLlib::Log::init();
	FLlib::Window::Create({1920,1080, "Window"});

	auto& running = FLlib::Globals::AppShouldRun;

	while (running) {
		if (FLlib::Input::OnKeyPressed(FLlib::Key::ESCAPE)) {
			running = false;
		}
		
		FLlib::Globals::timeStep.Update();

		
		
		FLlib::Globals::Window->OnUpdate();
	}
}
