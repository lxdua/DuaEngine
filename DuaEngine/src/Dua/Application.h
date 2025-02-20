#pragma once

#include "Dua/Core.h"
#include "Dua/Window.h"
#include "Dua/LayerStack.h"
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

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:

		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_running = true;
		LayerStack m_LayerStack;

	};

	Application* CreateApplication();
}


