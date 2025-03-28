#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D(const std::string& name) : Layer(name), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_Texture = Dua::Texture2D::Create("Assets/Textures/ntx.png");

    Dua::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Dua::Framebuffer::Create(fbSpec);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Dua::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

    m_Framebuffer->Bind();
	Dua::RenderCommand::SetClearColor({ 0.15,0.15,0.15,1.0 });
	Dua::RenderCommand::Clear();

	Dua::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Dua::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, 0.0f, m_Texture, m_SquareColor);

	for (float x = -5.0f; x <= 5.0f; x += 1)Dua::Renderer2D::DrawQuad({ x,-0.5f }, { 1.0f,1.0f }, 0, m_Texture, { 1.0f,1.0f,1.0f,1.0f });
	for (float x = -5.0f; x <= 5.0f; x += 1)Dua::Renderer2D::DrawQuad({ x,0.5f }, { 1.0f,1.0f }, 0, m_Texture, { (x + 5)/10.0f,0.5f,1.0f,0.5f });

	Dua::Renderer2D::EndScene();
    m_Framebuffer->UnBind();
}

void Sandbox2D::OnImGuiRender()
{

    ImGui::Begin("DockSpace Demo");

    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                Dua::Application::GetSingleton().Close();

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image(textureID, ImVec2{ 1280.0f,720.0f });

    ImGui::End();

}

void Sandbox2D::OnEvent(Dua::Event& event)
{
	m_CameraController.OnEvent(event);
}
