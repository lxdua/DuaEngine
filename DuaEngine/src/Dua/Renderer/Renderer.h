#pragma once

#include "RenderCommand.h"

namespace Dua {

	class Renderer
	{
	public:
		static RendererAPI::API GetAPI();

		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

	};

}