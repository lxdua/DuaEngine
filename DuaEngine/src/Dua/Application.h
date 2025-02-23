#pragma once

#include "Dua/Core.h"
#include "Dua/Window.h"
#include "Dua/LayerStack.h"
#include "Dua/ImGui/ImGuiLayer.h"
#include "Dua/Events/Event.h"
#include "Dua/Events/ApplicationEvent.h"
#include "Dua/Events/KeyEvent.h"
#include "Dua/Events/MouseEvent.h"

namespace Dua {

	class DUA_API Application
	{
	public:
		Application();
		virtual ~Application();

		inline static Application& GetSingleton();

		void Run();

		void OnEvent(Event& e);

		inline Window& GetWindow();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);

	};

	Application* CreateApplication();
}


