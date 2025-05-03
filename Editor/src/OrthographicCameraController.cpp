#include "duapch.h"
#include "OrthographicCameraController.h"

namespace Dua {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (!m_EnableControl)
			return;
		float actualSpeed = m_CameraTranslationSpeed * powf(m_ZoomLevel, 1.3f);

		if (Input::IsKeyPressed(DUA_KEY_A))
		{
			m_CameraPosition.x -= actualSpeed * ts;
		}
		else if (Input::IsKeyPressed(DUA_KEY_D))
		{
			m_CameraPosition.x += actualSpeed * ts;
		}

		if (Input::IsKeyPressed(DUA_KEY_W))
		{
			m_CameraPosition.y += actualSpeed * ts;
		}
		else if (Input::IsKeyPressed(DUA_KEY_S))
		{
			m_CameraPosition.y -= actualSpeed * ts;
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		if (!m_EnableControl)
			return;
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DUA_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DUA_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		if (height > 0)
			m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel = std::max(0.1f, m_ZoomLevel - e.GetYOffset() * 0.1f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		std::cout << "ZoomLevel: " << m_ZoomLevel << std::endl;
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}