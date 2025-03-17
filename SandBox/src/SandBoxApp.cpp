#include "duapch.h"
#include "Dua.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Dua::Layer
{
private:
	Dua::ShaderLibrary m_ShaderLib;
	Dua::Ref<Dua::Shader> m_Shader;
	Dua::Ref<Dua::VertexArray> m_VertexArray;

	Dua::Ref<Dua::Shader> m_SquareShader;
	Dua::Ref<Dua::VertexArray> m_SquareVA;

	Dua::Ref<Dua::Shader> m_TexShader;

	Dua::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePos;
	glm::vec4 m_SquareColor;

	Dua::Ref<Dua::Texture2D> m_Texture;
	Dua::Ref<Dua::Texture2D> m_NtxTex;

public:
	ExampleLayer() : Layer("Example"),
		m_CameraController(1280.0f / 720.0f),
		m_SquarePos(0.0f)
	{
	m_VertexArray.reset(Dua::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f, 
			 0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f
		};

		Dua::Ref<Dua::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Dua::VertexBuffer::Create(vertices, sizeof(vertices)));

		Dua::BufferLayout layout = {
			{ Dua::ShaderDataType::Float3, "a_pos" },
			{ Dua::ShaderDataType::Float4, "a_color" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Dua::Ref<Dua::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Dua::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		//m_Shader = Dua::Shader::Create("Assets/Shaders/tex.glsl");
		//m_Shader = m_ShaderLib.Load("Assets/Shaders/tex.glsl");
		m_ShaderLib.Load("Assets/Shaders/tex.glsl");
		m_Shader = m_ShaderLib.Get("tex");

		//////////////////////////////////////////////////

		m_SquareVA.reset(Dua::VertexArray::Create());

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

		std::string sq_vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_pos;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;	

			out vec3 v_pos;
			
			void main()
			{
				v_pos = a_pos;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_pos, 1.0);
			}
		)";

		std::string sq_fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_pos;

			uniform vec3 u_color;

			void main()
			{
				color = vec4(u_color, 1.0);
			}
		)";

		m_SquareShader = Dua::Shader::Create("sq", sq_vertexSrc, sq_fragmentSrc);


		///////////////////////////////////////////////

		std::string tex_vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_pos;
			layout(location = 1) in vec2 a_texcoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;	

			out vec2 v_texcoord;
			
			void main()
			{
				v_texcoord = a_texcoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_pos, 1.0);
			}
		)";

		std::string tex_fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_texcoord;

			uniform sampler2D u_texture;

			void main()
			{
				color = texture(u_texture, v_texcoord);
			}
		)";

		m_TexShader = Dua::Shader::Create("tex", tex_vertexSrc, tex_fragmentSrc);

		m_Texture = Dua::Texture2D::Create("Assets/Textures/tm.png");
		m_NtxTex = Dua::Texture2D::Create("Assets/Textures/ntx.png");
		
		std::dynamic_pointer_cast<Dua::OpenGLShader>(m_TexShader)->Bind();
		std::dynamic_pointer_cast<Dua::OpenGLShader>(m_TexShader)->UploadUniformInt("u_texture", 0);

	}

	void OnUpdate(Dua::Timestep delta) override
	{
		m_CameraController.OnUpdate(delta);
		//std::cout << "delta: " << ts.GetSecond() << std::endl;
		//std::cout << "fps: " << 1.0f / ts.GetSecond() << std::endl;

		//std::dynamic_pointer_cast<OpenGLShader>(shader)

		Dua::RenderCommand::SetClearColor({ 57 / 255.0, 197 / 255.0, 187 / 255.0, 1 });
		Dua::RenderCommand::Clear();

		if (Dua::Input::IsKeyPressed(DUA_KEY_A))
		{
			m_SquarePos.x -= 0.1f * delta;
		}
		if (Dua::Input::IsKeyPressed(DUA_KEY_D))
		{
			m_SquarePos.x += 0.1f * delta;
		}

		Dua::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 squaretrans = glm::translate(glm::mat4(1.0f), m_SquarePos);

		std::dynamic_pointer_cast<Dua::OpenGLShader>(m_SquareShader)->Bind();
		std::dynamic_pointer_cast<Dua::OpenGLShader>(m_SquareShader)->UploadUniformVec3("u_color", m_SquareColor);


		m_Texture->Bind();

		//Dua::Renderer::Submit(m_SquareShader, m_SquareVA, squaretrans);
		Dua::Renderer::Submit(m_TexShader, m_SquareVA, squaretrans);
		Dua::Renderer::Submit(m_Shader, m_VertexArray);

		Dua::Renderer::EndScene();
	}

	void OnEvent(Dua::Event& event) override
	{
		m_CameraController.OnEvent(event);
		//Dua::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Dua::KeyPressedEvent>(DUA_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Dua::KeyPressedEvent& event)
	{
		return false;
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}	

};

class Sandbox : public Dua::Application
{
public:

	Sandbox()
	{
		PushLayer(new ExampleLayer);
	}

	~Sandbox()
	{

	}
};

Dua::Application* Dua::CreateApplication()
{
	return new Sandbox();
}