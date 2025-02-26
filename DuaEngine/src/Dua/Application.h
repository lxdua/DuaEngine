#pragma once

#include "Dua/Window.h"

#include "Dua/LayerStack.h"
#include "Dua/ImGui/ImGuiLayer.h"

#include "Dua/Events/Event.h"
#include "Dua/Events/ApplicationEvent.h"
#include "Dua/Events/KeyEvent.h"
#include "Dua/Events/MouseEvent.h"

#include "Dua/Renderer/Shader.h"
#include "Dua/Renderer/Buffer.h"
#include "Dua/Renderer/VertexArray.h"

namespace Dua {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		static Application& GetSingleton();

		void Run();

		void OnEvent(Event& e);

		Window& GetWindow();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
	private:
		static Application* s_Instance;
		bool m_running = true;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		bool OnWindowClose(WindowCloseEvent& e);

	};

	Application* CreateApplication();
}


