#include "Window.h"

#include <glad/glad.h>

#include "Log.h"
#include "Globals.h"

namespace FLlib {
	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProp& data)
	{
		Init(data);
	}

	Window::~Window()
	{
		if (m_Window)
			glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
	void Window::Init(const WindowProp& data)
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!glfwInit())
		{
			LOG_FATAL("Failed to initialize GLFW!");
			return;
		}

		uint32_t width = data.Width;
		uint32_t height = data.Height;

		// Never open a window bigger than the monitor (keep 10% margin)
		if (GLFWmonitor* monitor = glfwGetPrimaryMonitor())
		{
			if (const GLFWvidmode* mode = glfwGetVideoMode(monitor))
			{
				uint32_t maxWidth = (uint32_t)(mode->width * 0.9f);
				uint32_t maxHeight = (uint32_t)(mode->height * 0.9f);
				if (width == 0 || width > maxWidth)   width = maxWidth;
				if (height == 0 || height > maxHeight) height = maxHeight;
			}
		}

		m_Prop.Width = width;
		m_Prop.Height = height;
		m_Prop.Title = data.Title;

		m_Window = glfwCreateWindow((int)width, (int)height, m_Prop.Title.c_str(), nullptr, nullptr);
		if (!m_Window)
		{
			LOG_FATAL("Failed to create GLFW window!");
			return;
		}

		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);
		SetVSync(true);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_FATAL("Failed to initialize Glad!");
			return;
		}

		LOG_INFO("OpenGL info:");
		LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		CenterWindow();
	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();

		auto color = Globals::clearColor;
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SetVSync(bool enabled)
	{
		m_Prop.VSync = enabled;
		glfwSwapInterval(enabled ? 1 : 0);
	}

	void Window::SetWindowPosition(int x, int y)
	{
		glfwSetWindowPos(m_Window, x, y);
	}

	void Window::CenterWindow()
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor) return;
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!mode) return;

		int xpos = (mode->width - (int)m_Prop.Width) / 2;
		int ypos = (mode->height - (int)m_Prop.Height) / 2;
		glfwSetWindowPos(m_Window, xpos, ypos);
	}

	std::shared_ptr<Window> Window::Create(const WindowProp& prop)
	{
		auto window = std::make_shared<Window>(prop);
		Globals::Window = window;
		return window;
	}
}