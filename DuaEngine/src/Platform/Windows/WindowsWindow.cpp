#include "duapch.h"
#include "WindowsWindow.h"

#include "Dua/Events/ApplicationEvent.h"
#include "Dua/Events/KeyEvent.h"
#include "Dua/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace Dua {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) { return new WindowsWindow(props); }
	
	WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

	WindowsWindow::~WindowsWindow() { Shutdown(); }

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		std::cout << "Creating window: " << props.Title << "(" << props.Width << ", " << props.Height << ")" << std::endl;
	
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			
			std::cout
				<< success
				<< (success ? " GLFW was successfully initialized" : " Could not initialize GLFW!")
				<< std::endl;

			glfwSetErrorCallback([](int error_code, const char* description)
				{
					std::cout << "GLFW Error (" << error_code << "): " << description << std::endl;
				}
			);
			s_GLFWInitialized = true;
		}
		
		m_Window = glfwCreateWindow(
			(int)props.Width,
			(int)props.Height,
			props.Title.c_str(),
			nullptr,
			nullptr
		);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(
			m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event(width, height);
				data.EventCallback(event);
				data.Width = width;
				data.Height = height;
			}
		);

		glfwSetWindowCloseCallback(
			m_Window,
			[](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			}
		);

		glfwSetKeyCallback(
			m_Window,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, scancode, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key, scancode);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, scancode, 1);
						data.EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetMouseButtonCallback(
			m_Window,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetScrollCallback(
			m_Window,
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			}
		);

		glfwSetCursorPosCallback(
			m_Window,
			[](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			}
		);

	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}