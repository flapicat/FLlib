#include "Input.h"

#include <GLFW/glfw3.h>

namespace FLlib
{
	std::array<Input::ButtonState, Input::KeyCount> Input::s_Keys{};
	std::array<Input::ButtonState, Input::MouseButtonCount> Input::s_MouseButtons{};

	glm::vec2 Input::s_MousePosition = { 0.0f, 0.0f };
	glm::vec2 Input::s_MouseDelta = { 0.0f, 0.0f };
	glm::vec2 Input::s_ScrollDelta = { 0.0f, 0.0f };
	bool Input::s_HasMousePosition = false;

	template<size_t N>
	const Input::ButtonState* Input::Find(const std::array<ButtonState, N>& states, int index)
	{
		if (index < 0 || (size_t)index >= N)
			return nullptr;
		return &states[index];
	}

	bool Input::IsKeyPressed(Key keycode)
	{
		auto state = Find(s_Keys, (int)keycode);
		return state && state->Down;
	}

	bool Input::IsMouseButtonPressed(uint32_t button)
	{
		auto state = Find(s_MouseButtons, (int)button);
		return state && state->Down;
	}

	bool Input::OnKeyPressed(Key keycode)
	{
		auto state = Find(s_Keys, (int)keycode);
		return state && state->Pressed;
	}

	bool Input::OnKeyReleased(Key keycode)
	{
		auto state = Find(s_Keys, (int)keycode);
		return state && state->Released;
	}

	bool Input::OnMouseButtonPressed(uint32_t button)
	{
		auto state = Find(s_MouseButtons, (int)button);
		return state && state->Pressed;
	}

	bool Input::OnMouseButtonReleased(uint32_t button)
	{
		auto state = Find(s_MouseButtons, (int)button);
		return state && state->Released;
	}

	glm::vec2 Input::GetMousePosition() { return s_MousePosition; }
	glm::vec2 Input::GetMouseDelta() { return s_MouseDelta; }
	glm::vec2 Input::GetScrollDelta() { return s_ScrollDelta; }

	void Input::BeginFrame()
	{
		for (auto& key : s_Keys)
			key.Pressed = key.Released = false;
		for (auto& button : s_MouseButtons)
			button.Pressed = button.Released = false;

		s_MouseDelta = { 0.0f, 0.0f };
		s_ScrollDelta = { 0.0f, 0.0f };
	}

	template<size_t N>
	void Input::Apply(std::array<ButtonState, N>& states, int index, int action)
	{
		if (index < 0 || (size_t)index >= N)
			return;

		auto& state = states[index];
		if (action == GLFW_PRESS)
		{
			state.Down = true;
			state.Pressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			state.Down = false;
			state.Released = true;
		}
	}

	void Input::OnKey(int key, int action)
	{
		Apply(s_Keys, key, action);
	}

	void Input::OnMouseButton(int button, int action)
	{
		Apply(s_MouseButtons, button, action);
	}

	void Input::OnMouseMove(float x, float y)
	{
		const glm::vec2 position = { x, y };
		if (s_HasMousePosition)
			s_MouseDelta += position - s_MousePosition;
		s_MousePosition = position;
		s_HasMousePosition = true;
	}

	void Input::ResetMouseTracking()
	{
		s_HasMousePosition = false;
	}

	void Input::OnScroll(float x, float y)
	{
		s_ScrollDelta += glm::vec2(x, y);
	}
}
