#pragma once

#include "RenderCommand.h"

#include "Shader.h"
#include "OrthographicCamera.h"

namespace Dua {

	class Renderer
	{
	public:
		static RendererAPI::API GetAPI();

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f)
		);

	private:
		static glm::mat4 m_VPMatrix;
	};

}