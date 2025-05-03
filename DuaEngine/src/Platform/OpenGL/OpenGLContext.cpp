#include "duapch.h"
#include "OpenGLContext.h"

namespace Dua {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : m_Window(window) {}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		std::cout
			<< status
			<< (status ? " Glad was successfully initialized" : " Failed to initialize Glad!")
			<< std::endl;
		std::cout
			<< "OpenGL Info:\n"
			<< " Vendor: " << glGetString(GL_VENDOR) << '\n'
			<< " Renderer: " << glGetString(GL_RENDERER) << '\n'
			<< " Version: " << glGetString(GL_VERSION) << std::endl;
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

}