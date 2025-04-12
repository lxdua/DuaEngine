#pragma once

#include "Dua.h"
#include "OrthographicCameraController.h"

namespace Dua {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::string& name = "EditorLayer");
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:

		Ref<Scene> m_Scene;

		OrthographicCameraController m_CameraController;
		ShaderLibrary m_ShaderLib;

		glm::vec2 m_ViewportSize = { 1280.0f,720.0f };

		Ref<Shader> m_SquareShader;
		Ref<VertexArray> m_SquareVA;

		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture;

		glm::vec4 m_SquareColor;
	};

}