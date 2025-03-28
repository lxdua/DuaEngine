#include "duapch.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Dua {

	glm::mat4 Renderer::m_VPMatrix = *(new glm::mat4);

	RendererAPI::API Renderer::GetAPI()
	{
		return RendererAPI::GetAPI();
	}

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		//RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_VPMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		
		shader->SetMat4("u_ViewProjection", m_VPMatrix);
		shader->SetMat4("u_Transform", transform);
		
		vertexArray->Bind();
		//RenderCommand::DrawIndexed(vertexArray);
	}

}