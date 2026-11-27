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

		WindowProp(uint32_t width = 1600, uint32_t height = 900, const std::string& title = "FL_Engine")
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

		static std::shared_ptr<Window> Create(const WindowProp& prop);
	private:
		void Init(const WindowProp& prop);
	private:
		GLFWwindow* m_Window = nullptr;
		WindowProp m_Prop;
	};
	

}