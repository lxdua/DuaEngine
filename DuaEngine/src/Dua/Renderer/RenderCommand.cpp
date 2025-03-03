#include "duapch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dua {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		s_RendererAPI->DrawIndexed(vertexArray);
	}

}