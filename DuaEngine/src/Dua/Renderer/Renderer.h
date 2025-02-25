#pragma once

#include "Dua/Core.h"

namespace Dua {

	enum class RendererAPI
	{
		None = 0, OpenGL, Direct3D, Vulkan
	};

	class DUA_API Renderer
	{
	public:
		inline static RendererAPI GetRendererAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

	

}