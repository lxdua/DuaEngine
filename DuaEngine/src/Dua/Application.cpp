#include "duapch.h"
#include "Application.h"

#include "Dua/Input.h"

#include "GLM/glm.hpp"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Dua {

#define DUA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		//case ShaderDataType::Mat3: return GL_FLOAT;
		//case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Bool: return GL_BOOL;
		}
	}

	Application::Application()
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(DUA_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
		

		// 生成并绑定顶点数组对象(VAO)，用于存储顶点属性配置
		// VAO可以一次性保存多个VBO和顶点属性指针的配置，方便后续快速切换
		glGenVertexArrays(1, &m_VertexArray); // 生成1个VAO
		glBindVertexArray(m_VertexArray);     // 绑定VAO，后续操作将作用于这个VAO


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f, 
			 0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_pos" },
			{ ShaderDataType::Float4, "a_color" }
		});

		uint32_t index = 0;
		const BufferLayout& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout.GetElements())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		unsigned int indices[3] = { 0,1,2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
		
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_pos;
			layout(location = 1) in vec4 a_color;

			out vec3 v_pos;
			out vec4 v_color;
			
			void main()
			{
				v_pos = a_pos;
				v_color = a_color;
				gl_Position = vec4(a_pos, 1.0);
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
			glClearColor(57/255.0, 197/255.0, 187/255.0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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