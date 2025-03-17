#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D(const std::string& name) : Layer(name), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Dua::VertexArray::Create();

	float sq_vertices[4 * 5] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
	};

	Dua::Ref<Dua::VertexBuffer> squareVB;
	squareVB.reset(Dua::VertexBuffer::Create(sq_vertices, sizeof(sq_vertices)));
	Dua::BufferLayout sq_layout = {
		{ Dua::ShaderDataType::Float3, "a_pos" },
		{ Dua::ShaderDataType::Float2, "a_texcoord" }
	};
	squareVB->SetLayout(sq_layout);
	m_SquareVA->AddVertexBuffer(squareVB);

	unsigned int sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
	Dua::Ref<Dua::IndexBuffer> squareIB;
	squareIB.reset(Dua::IndexBuffer::Create(sq_indices, 6));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_SquareShader = m_ShaderLib.Load("Assets/Shaders/sq.glsl");

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Dua::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Dua::RenderCommand::SetClearColor({ 57 / 255.0, 197 / 255.0, 187 / 255.0, 1 });
	Dua::RenderCommand::Clear();

	Dua::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Dua::OpenGLShader>(m_SquareShader)->Bind();
	std::dynamic_pointer_cast<Dua::OpenGLShader>(m_SquareShader)->UploadUniformVec3("u_color", m_SquareColor);

	m_SquareShader->Bind();

	Dua::Renderer::Submit(m_SquareShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Dua::Renderer::EndScene();
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
