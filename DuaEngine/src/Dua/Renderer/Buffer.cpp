#include "duapch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
//#include "Platform/Direct3D/Direct3DBuffer.h"

namespace Dua {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		case RendererAPI::API::Direct3D:
			return nullptr;
		}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		case RendererAPI::API::Direct3D:
			return nullptr;
		}
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		case RendererAPI::API::Direct3D:
			return nullptr;
		}
		return nullptr;
	}

}