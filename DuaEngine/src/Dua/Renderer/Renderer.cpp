#include "duapch.h"
#include "Renderer.h"

namespace Dua {

	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

	RendererAPI Renderer::GetAPI()
	{
		return s_RendererAPI;
	}

}