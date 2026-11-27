#include "Input.h"

#include <GLFW/glfw3.h>

#include "Globals.h"
namespace FLlib
{
	std::unordered_map<Key, bool> Input::s_PrevKeyState;
	std::unordered_map<uint32_t, bool> Input::s_PrevMouseButtonState;

	bool Input::IsKeyPressed(Key keycode)
	{
		auto window = Globals::Window->GetNativeWindow();
		return glfwGetKey(window, (int)keycode) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(uint32_t button)
	{
		auto window = Globals::Window->GetNativeWindow();
		return glfwGetMouseButton(window, (int)button) == GLFW_PRESS;
	}

	bool Input::OnKeyPressed(Key keycode)
	{
		bool wasPressed = s_PrevKeyState[keycode];
		return IsKeyPressed(keycode) && !wasPressed;
	}

	bool Input::OnMouseButtonPressed(uint32_t  button)
	{
		bool wasPressed = s_PrevMouseButtonState[button];
		return IsMouseButtonPressed(button) && !wasPressed;
	}
}
