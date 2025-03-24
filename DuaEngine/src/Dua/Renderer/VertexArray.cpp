#include "duapch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Dua {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		case RendererAPI::API::Direct3D:
			return nullptr;
		}
		return nullptr;
	}

}