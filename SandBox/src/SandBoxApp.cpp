#include "duapch.h"
#include "Dua.h"


class ExampleLayer : public Dua::Layer
{
private:

	std::shared_ptr<Dua::Shader> m_Shader;
	std::shared_ptr<Dua::VertexArray> m_VertexArray;

	std::shared_ptr<Dua::Shader> m_SquareShader;
	std::shared_ptr<Dua::VertexArray> m_SquareVA;

	Dua::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPos;

public:
	ExampleLayer() : Layer("Example"),
		m_Camera(-1.28f, 1.28f, -0.72f, 0.72f),
		m_CameraPos(0.0f)
	{
	m_VertexArray.reset(Dua::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f, 
			 0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Dua::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Dua::VertexBuffer::Create(vertices, sizeof(vertices)));

		Dua::BufferLayout layout = {
			{ Dua::ShaderDataType::Float3, "a_pos" },
			{ Dua::ShaderDataType::Float4, "a_color" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<Dua::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Dua::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_pos;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_ViewProjection;

			out vec3 v_pos;
			out vec4 v_color;
			
			void main()
			{
				v_pos = a_pos;
				v_color = a_color;
				gl_Position = u_ViewProjection * vec4(a_pos, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_pos;
			in vec4 v_color;

			void main()
			{
				color = v_color;
			}
		)";

		m_Shader.reset(new Dua::Shader(vertexSrc, fragmentSrc));

		//////////////////////////////////////////////////

		m_SquareVA.reset(Dua::VertexArray::Create());

		float sq_vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Dua::VertexBuffer> squareVB;
		squareVB.reset(Dua::VertexBuffer::Create(sq_vertices, sizeof(sq_vertices)));
		Dua::BufferLayout sq_layout = {
			{ Dua::ShaderDataType::Float3, "a_pos" }
		};
		squareVB->SetLayout(sq_layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Dua::IndexBuffer> squareIB;
		squareIB.reset(Dua::IndexBuffer::Create(sq_indices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string sq_vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_pos;

			uniform mat4 u_ViewProjection;			

			out vec3 v_pos;
			
			void main()
			{
				v_pos = a_pos;
				gl_Position = u_ViewProjection * vec4(a_pos, 1.0);
			}
		)";

		std::string sq_fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_pos;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_SquareShader.reset(new Dua::Shader(sq_vertexSrc, sq_fragmentSrc));
	}

	void OnUpdate(Dua::Timestep ts) override
	{
		std::cout << "delta: " << ts.GetSecond() << std::endl;
		std::cout << "fps: " << 1.0f / ts.GetSecond() << std::endl;

		Dua::RenderCommand::SetClearColor({ 57 / 255.0, 197 / 255.0, 187 / 255.0, 1 });
		Dua::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetRotaetion(45.0f);

		Dua::Renderer::BeginScene(m_Camera);

		Dua::Renderer::Submit(m_SquareShader, m_SquareVA);
		Dua::Renderer::Submit(m_Shader, m_VertexArray);

		Dua::Renderer::EndScene();
	}

	void OnEvent(Dua::Event& event) override
	{
		Dua::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Dua::KeyPressedEvent>(DUA_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Dua::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == DUA_KEY_LEFT)
		{
			m_CameraPos.x -= 0.1f;
		}
		if (event.GetKeyCode() == DUA_KEY_RIGHT)
		{
			m_CameraPos.x += 0.1f;
		}
		return true;
	}

	void OnImGuiRender() override
	{

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