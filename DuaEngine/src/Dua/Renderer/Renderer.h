#pragma once

#include "RenderCommand.h"

#include "Shader.h"
#include "OrthographicCamera.h"

namespace Dua {

	class Renderer
	{
	public:
		static RendererAPI::API GetAPI();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f)
		);

	private:
		static glm::mat4 m_VPMatrix;
	};

}