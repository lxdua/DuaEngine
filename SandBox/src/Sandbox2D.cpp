#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D(const std::string& name) : Layer(name), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_Texture = Dua::Texture2D::Create("Assets/Textures/ntx.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Dua::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Dua::RenderCommand::SetClearColor({ 57 / 255.0, 197 / 255.0, 187 / 255.0, 1 });
	Dua::RenderCommand::Clear();

	Dua::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//std::dynamic_pointer_cast<Dua::OpenGLShader>(m_SquareShader)->Bind();
	//std::dynamic_pointer_cast<Dua::OpenGLShader>(m_SquareShader)->UploadUniformVec3("u_color", m_SquareColor);
	//m_SquareShader->Bind();

	//Dua::Renderer::Submit(m_SquareShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Dua::Renderer2D::DrawQuad({ 1.0f,2.0f }, { 0.5f,1.0f }, { 0.5f,0.2f,0.3f,1.0f });
	Dua::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, m_Texture);

	Dua::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Dua::Event& event)
{
}
