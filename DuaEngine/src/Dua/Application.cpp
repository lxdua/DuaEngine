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
		
		unsigned int id;
		glGenVertexArrays(1, &id);
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