#include "duapch.h"
#include "Application.h"

#include "Dua/Input.h"

#include "GLM/glm.hpp"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Dua {

	Application* Application::s_Instance = nullptr;

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

		// 定义三角形的三个顶点坐标（x,y,z）
		// 每个顶点3个float值，共3个顶点组成一个三角形
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,  // 左下顶点
			 0.5f, -0.5f, 0.0f,  // 右下顶点
			 0.0f,  0.5f, 0.0f   // 顶部顶点
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


		// 配置顶点属性指针（告诉OpenGL如何解析顶点数据）
		// 位置属性（location = 0），每个顶点由3个float值组成（x,y,z）
		glEnableVertexAttribArray(0); // 启用顶点属性位置0
		glVertexAttribPointer(
			0,                 // 属性位置索引（对应shader中的location）
			3,                 // 每个顶点的分量数量（x,y,z）
			GL_FLOAT,          // 数据类型
			GL_FALSE,          // 是否标准化（不需要）
			3 * sizeof(float), // 步长（连续顶点之间的字节偏移）
			nullptr             // 数据起始位置的偏移量（0表示从缓冲区开始）
		);

		unsigned int indices[3] = { 0,1,2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));

		
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 pos;
			out vec3 o_pos;
			
			void main()
			{
				o_pos = pos * -1;
				gl_Position = vec4(pos * -1, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 o_pos;

			void main()
			{
				color = vec4(o_pos * 0.5 + 0.5, 1.0);
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