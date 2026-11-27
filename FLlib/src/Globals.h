#pragma once
#include <memory>

#include "TimeStep.h"
#include "Window.h"

#include <glm/glm.hpp>

namespace FLlib {
	struct Globals
	{
		inline static bool AppShouldRun = true;
		inline static TimeStep timeStep;
		inline static glm::vec4 clearColor = { 0.1, 0.1f, 0.1f, 1.0f };
		inline static std::shared_ptr<Window> Window = nullptr;
	};

}