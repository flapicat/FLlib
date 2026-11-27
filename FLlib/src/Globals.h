#pragma once
#include <memory>

#include "TimeStep.h"
#include "Window.h"

#include "PerspectiveCamera.h"

#include <glm/glm.hpp>

namespace FLlib {
	struct Globals
	{
		inline static bool AppShouldRun = true;
		inline static TimeStep timeStep;
		inline static glm::vec4 clearColor = { 0.1, 0.1f, 0.1f, 1.0f };
		inline static std::shared_ptr<Window> Window = nullptr;
		inline static PerspectiveCamera Camera;
	};

    struct Color
    {
        inline static constexpr glm::vec4 Black = { 0.000f, 0.000f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 White = { 1.000f, 1.000f, 1.000f, 1.0f };
        inline static constexpr glm::vec4 Red = { 1.000f, 0.000f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 Lime = { 0.000f, 1.000f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 Blue = { 0.000f, 0.000f, 1.000f, 1.0f };
        inline static constexpr glm::vec4 Yellow = { 1.000f, 1.000f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 Cyan = { 0.000f, 1.000f, 1.000f, 1.0f };
        inline static constexpr glm::vec4 Magenta = { 1.000f, 0.000f, 1.000f, 1.0f };

        inline static constexpr glm::vec4 Silver = { 0.753f, 0.753f, 0.753f, 1.0f };
        inline static constexpr glm::vec4 Gray = { 0.502f, 0.502f, 0.502f, 1.0f };
        inline static constexpr glm::vec4 Maroon = { 0.502f, 0.000f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 Olive = { 0.502f, 0.502f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 Green = { 0.000f, 0.502f, 0.000f, 1.0f };
        inline static constexpr glm::vec4 Purple = { 0.502f, 0.000f, 0.502f, 1.0f };
        inline static constexpr glm::vec4 Teal = { 0.000f, 0.502f, 0.502f, 1.0f };
        inline static constexpr glm::vec4 Navy = { 0.000f, 0.000f, 0.502f, 1.0f };
    };
}