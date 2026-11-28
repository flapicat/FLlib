#pragma once
#include <memory>
#include <cstdint>
#include <string>

#include <GLFW/glfw3.h>

namespace FLlib {
	struct WindowProp
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;
		bool VSync = true;

		WindowProp(uint32_t width = 1600, uint32_t height = 900, const std::string& title = "FLlib")
			:Width(width), Height(height), Title(title)
		{
		}
	};

	class Window
	{
	public:
		Window(const WindowProp& prop);
		~Window();

		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Prop.Width; }
		inline uint32_t GetHeight() const { return m_Prop.Height; }

		void SetVSync(bool enabled);
		bool IsVSync() const { return m_Prop.VSync; }

		GLFWwindow* GetNativeWindow() const { return m_Window; }

		void SetWindowPosition(int x, int y);
		void CenterWindow();

		void SetCursorLocked(bool locked);
		bool IsCursorLocked() const { return m_CursorLocked; }

		static std::shared_ptr<Window> Create(const WindowProp& prop);
	private:
		void Init(const WindowProp& prop);
		void SetCallbacks();

		static void CloseCallback(GLFWwindow* window);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void FocusCallback(GLFWwindow* window, int focused);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, unsigned int codepoint);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double x, double y);
		static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	private:
		GLFWwindow* m_Window = nullptr;
		WindowProp m_Prop;
		bool m_CursorLocked = false;
	};
	

}