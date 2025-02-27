#include "duapch.h"
#include "RendererAPI.h"

namespace Dua {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	RendererAPI::API RendererAPI::GetAPI()
	{
		return s_API;
	}

}