#pragma once
#include <array>
#include <cstdint>

#include <glm/glm.hpp>

#include "KeyCodes.h"

namespace FLlib
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key keycode);
		static bool IsMouseButtonPressed(uint32_t button);

		static bool OnKeyPressed(Key keycode);
		static bool OnKeyReleased(Key keycode);
		static bool OnMouseButtonPressed(uint32_t button);
		static bool OnMouseButtonReleased(uint32_t button);

		static glm::vec2 GetMousePosition();
		static glm::vec2 GetMouseDelta();
		static glm::vec2 GetScrollDelta();
	private:
		static constexpr size_t KeyCount = 349;
		static constexpr size_t MouseButtonCount = 8;

		struct ButtonState
		{
			bool Down = false;
			bool Pressed = false;
			bool Released = false;
		};

		friend class Window;
		static void BeginFrame();
		static void OnKey(int key, int action);
		static void OnMouseButton(int button, int action);
		static void OnMouseMove(float x, float y);
		static void OnScroll(float x, float y);
		static void ResetMouseTracking();

		template<size_t N>
		static const ButtonState* Find(const std::array<ButtonState, N>& states, int index);
		template<size_t N>
		static void Apply(std::array<ButtonState, N>& states, int index, int action);
	private:

		static std::array<ButtonState, KeyCount> s_Keys;
		static std::array<ButtonState, MouseButtonCount> s_MouseButtons;

		static glm::vec2 s_MousePosition;
		static glm::vec2 s_MouseDelta;
		static glm::vec2 s_ScrollDelta;
		static bool s_HasMousePosition;
	};
}
