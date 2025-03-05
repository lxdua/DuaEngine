#include "duapch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Dua {

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filepath);
		case RendererAPI::API::Direct3D:
			return nullptr;
		}
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSource, std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSource, fragmentSource);
		case RendererAPI::API::Direct3D:
			return nullptr;
		}
		return nullptr;
	}

}