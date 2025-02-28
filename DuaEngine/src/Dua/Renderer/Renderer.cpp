#include "duapch.h"
#include "Renderer.h"

namespace Dua {

	glm::mat4 Renderer::m_VPMatrix = *(new glm::mat4);

	RendererAPI::API Renderer::GetAPI()
	{
		return RendererAPI::GetAPI();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_VPMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_VPMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}