#pragma once
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "KeyCodes.h"

namespace FLlib
{
	enum class KeyAction { Pressed, Released, Repeat };

	enum KeyMod : uint32_t
	{
		Mod_Shift    = 0x0001,
		Mod_Control  = 0x0002,
		Mod_Alt      = 0x0004,
		Mod_Super    = 0x0008,
		Mod_CapsLock = 0x0010,
		Mod_NumLock  = 0x0020,
	};

	struct WindowCloseEvent {};
	struct WindowResizeEvent { uint32_t Width; uint32_t Height; };
	struct WindowFocusEvent { bool Focused; };

	struct KeyEvent { Key Code; KeyAction Action; uint32_t Mods; };
	struct CharEvent { uint32_t Codepoint; };

	struct MouseButtonEvent { uint32_t Button; bool Pressed; uint32_t Mods; };
	struct MouseMoveEvent { float X; float Y; };
	struct MouseScrollEvent { float XOffset; float YOffset; };

	using Event = std::variant<
		WindowCloseEvent,
		WindowResizeEvent,
		WindowFocusEvent,
		KeyEvent,
		CharEvent,
		MouseButtonEvent,
		MouseMoveEvent,
		MouseScrollEvent
	>;

	template<class... Ts>
	struct Overloaded : Ts... { using Ts::operator()...; };

	class Events
	{
	public:
		static std::optional<Event> Poll();
	private:
		friend class Window;
		static void Push(const Event& event);
		static void Clear();
	private:
		static std::vector<Event> s_Queue;
		static size_t s_ReadIndex;
	};
}
