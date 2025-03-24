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

	Dua::RenderCommand::SetClearColor({ 0.15,0.15,0.15,1.0 });
	Dua::RenderCommand::Clear();

	Dua::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Dua::Renderer2D::DrawQuad({ 1.0f,2.0f }, { 0.5f,1.0f }, { 0.5f,0.2f,0.3f,1.0f });
	Dua::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, 0.0f, m_Texture, glm::vec4(1.0f, 0.5f, 1.0f, 0.5f));

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
	m_CameraController.OnEvent(event);
}
