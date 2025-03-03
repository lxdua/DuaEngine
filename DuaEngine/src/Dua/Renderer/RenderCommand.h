#pragma once

#include "RendererAPI.h"

#include "Dua/Core/Core.h"

namespace Dua {

	class RenderCommand
	{
	public:

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const Ref<VertexArray>& vertexArray);

	private:
		static RendererAPI* s_RendererAPI;
	};

}