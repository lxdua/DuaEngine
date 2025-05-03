#pragma once

#include "Dua/Renderer/GraphicsContext.h"

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace Dua {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;

	};


}
