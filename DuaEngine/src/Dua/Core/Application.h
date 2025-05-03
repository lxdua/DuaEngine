#pragma once

#include "Dua/Core/Window.h"
#include "Dua/Core/Memory.h"
#include "Dua/Core/Timestep.h"
#include "Dua/Events/Event.h"
#include "Dua/Layer/LayerStack.h"
#include "Dua/ImGui/ImGuiLayer.h"


//#include "Dua/Core/Input/Input.h"
//#include "Dua/Core/Input/InputCodes.h"
//#include "Dua/Events/ApplicationEvent.h"
//#include "Dua/Events/KeyEvent.h"
//#include "Dua/Events/MouseEvent.h"
//#include "Dua/Renderer/Shader.h"
//#include "Dua/Renderer/Buffer.h"
//#include "Dua/Renderer/VertexArray.h"
//#include "Dua/Renderer/OrthographicCamera.h"

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

		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
	private:
		static Application* s_Instance;
		bool m_Running = true;
		bool m_Minimized = false;

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		
		float m_LastFrameTime = 0.0f;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	};

	Application* CreateApplication();
}


