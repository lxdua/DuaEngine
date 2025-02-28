#include "duapch.h"
#include "Application.h"

#include "Dua/Input.h"
#include "Dua/Renderer/Renderer.h"

#include "GLM/glm.hpp"

namespace Dua {

#define DUA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Camera(-1.28f, 1.28f, -0.72f, 0.72f)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(DUA_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		//////////////////////////////////////////////////

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f, 
			 0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_pos" },
			{ ShaderDataType::Float4, "a_color" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//////////////////////////////////////////////////

		m_SquareVA.reset(VertexArray::Create());

		float sq_vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(sq_vertices, sizeof(sq_vertices)));
		BufferLayout sq_layout = {
			{ ShaderDataType::Float3, "a_pos" }
		};
		squareVB->SetLayout(sq_layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(sq_indices, 6));
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

		m_SquareShader.reset(new Shader(sq_vertexSrc, sq_fragmentSrc));
	}

	Application::~Application()
	{

	}

	Application& Application::GetSingleton()
	{
		return *s_Instance;
	}

	void Application::Run()
	{
		while (m_running)
		{
			RenderCommand::SetClearColor({ 57 / 255.0, 197 / 255.0, 187 / 255.0, 1 });
			RenderCommand::Clear();

			m_Camera.SetRotaetion(45.0f);
			m_Camera.SetPosition({ 0.7f, 0.0f, 0.0f });

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_SquareShader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DUA_BIND_EVENT_FN(Application::OnWindowClose));

		std::cout << e.ToString() << std::endl;

		for (std::vector<Dua::Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

}