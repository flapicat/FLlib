#pragma once
#include <cstdint>
#include <unordered_map>
#include "KeyCodes.h"

namespace FLlib
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key keycode);
		static bool IsMouseButtonPressed(uint32_t button);

		static bool OnKeyPressed(Key keycode);
		static bool OnMouseButtonPressed(uint32_t button);
	private:
		static std::unordered_map<Key, bool> s_PrevKeyState;
		static std::unordered_map<uint32_t, bool> s_PrevMouseButtonState;
	};
}
