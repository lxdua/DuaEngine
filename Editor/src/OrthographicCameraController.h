#pragma once

#include "Dua/Renderer/OrthographicCamera.h"
#include "Dua/Core/Timestep.h"

#include "Dua/Core/Input/Input.h"
#include "Dua/Core/Input/InputCodes.h"

#include "Dua/Events/ApplicationEvent.h"
#include "Dua/Events/KeyEvent.h"
#include "Dua/Events/MouseEvent.h"

namespace Dua {

	class ViewportPanel;

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		OrthographicCamera m_Camera;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraTranslationSpeed = 1.0f;

		bool m_EnableControl = false;

		friend class ViewportPanel;
	};

}