#pragma once

#include "Dua/Core.h"

#include "Dua/Window.h"

#include "Dua/LayerStack.h"
#include "Dua/ImGui/ImGuiLayer.h"

#include "Dua/Events/Event.h"
#include "Dua/Events/ApplicationEvent.h"
#include "Dua/Events/KeyEvent.h"
#include "Dua/Events/MouseEvent.h"

#include "Dua/Renderer/Shader.h"
#include "Dua/Renderer/Buffer.h"

namespace Dua {

	class DUA_API Application
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

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		bool OnWindowClose(WindowCloseEvent& e);

	};

	Application* CreateApplication();
}


