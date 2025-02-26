#pragma once

namespace Dua {

	enum class RendererAPI
	{
		None = 0, OpenGL, Direct3D, Vulkan
	};

	class Renderer
	{
	public:
		static RendererAPI GetAPI();
	private:
		static RendererAPI s_RendererAPI;
	};

	

}