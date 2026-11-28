#include "Window.h"

#include <glad/glad.h>

#include "Log.h"
#include "Globals.h"
#include "Events.h"
#include "Input.h"

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

		int fbWidth = 0, fbHeight = 0;
		glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
		glViewport(0, 0, fbWidth, fbHeight);

		SetCallbacks();
		CenterWindow();
	}

	void Window::SetCallbacks()
	{
		glfwSetWindowCloseCallback(m_Window, CloseCallback);
		glfwSetWindowSizeCallback(m_Window, WindowSizeCallback);
		glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
		glfwSetWindowFocusCallback(m_Window, FocusCallback);
		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetCharCallback(m_Window, CharCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetCursorPosCallback(m_Window, CursorPosCallback);
		glfwSetScrollCallback(m_Window, ScrollCallback);
	}

	void Window::CloseCallback(GLFWwindow*)
	{
		Globals::AppShouldRun = false;
		Events::Push(WindowCloseEvent{});
	}

	void Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		auto self = (Window*)glfwGetWindowUserPointer(window);
		self->m_Prop.Width = (uint32_t)width;
		self->m_Prop.Height = (uint32_t)height;
	}

	void Window::FramebufferSizeCallback(GLFWwindow*, int width, int height)
	{
		glViewport(0, 0, width, height);
		if (width > 0 && height > 0)
			Globals::Camera.SetAspectRatio((float)width / (float)height);
		Events::Push(WindowResizeEvent{ (uint32_t)width, (uint32_t)height });
	}

	void Window::FocusCallback(GLFWwindow*, int focused)
	{
		Events::Push(WindowFocusEvent{ focused == GLFW_TRUE });
	}

	void Window::KeyCallback(GLFWwindow*, int key, int, int action, int mods)
	{
		Input::OnKey(key, action);

		KeyAction keyAction = action == GLFW_PRESS ? KeyAction::Pressed
			: action == GLFW_RELEASE ? KeyAction::Released
			: KeyAction::Repeat;
		Events::Push(KeyEvent{ (Key)key, keyAction, (uint32_t)mods });
	}

	void Window::CharCallback(GLFWwindow*, unsigned int codepoint)
	{
		Events::Push(CharEvent{ codepoint });
	}

	void Window::MouseButtonCallback(GLFWwindow*, int button, int action, int mods)
	{
		Input::OnMouseButton(button, action);
		Events::Push(MouseButtonEvent{ (uint32_t)button, action == GLFW_PRESS, (uint32_t)mods });
	}

	void Window::CursorPosCallback(GLFWwindow*, double x, double y)
	{
		Input::OnMouseMove((float)x, (float)y);
		Events::Push(MouseMoveEvent{ (float)x, (float)y });
	}

	void Window::ScrollCallback(GLFWwindow*, double xOffset, double yOffset)
	{
		Input::OnScroll((float)xOffset, (float)yOffset);
		Events::Push(MouseScrollEvent{ (float)xOffset, (float)yOffset });
	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);

		Input::BeginFrame();
		Events::Clear();
		glfwPollEvents();

		auto color = Globals::clearColor;
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	void Window::SetCursorLocked(bool locked)
	{
		if (locked == m_CursorLocked)
			return;

		m_CursorLocked = locked;
		glfwSetInputMode(m_Window, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, locked ? GLFW_TRUE : GLFW_FALSE);

		Input::ResetMouseTracking();
	}

	std::shared_ptr<Window> Window::Create(const WindowProp& prop)
	{
		auto window = std::make_shared<Window>(prop);
		Globals::Window = window;
		return window;
	}
}