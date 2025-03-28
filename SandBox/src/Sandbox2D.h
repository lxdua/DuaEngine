#pragma once

#include "Dua.h"

class Sandbox2D : public Dua::Layer
{
public:
	Sandbox2D(const std::string& name = "Sandbox2D");
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Dua::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Dua::Event& event) override;

private:
	Dua::OrthographicCameraController m_CameraController;
	Dua::ShaderLibrary m_ShaderLib;
	Dua::Ref<Dua::Shader> m_SquareShader;
	Dua::Ref<Dua::VertexArray> m_SquareVA;

	Dua::Ref<Dua::Framebuffer> m_Framebuffer;

	Dua::Ref<Dua::Texture2D> m_Texture;

	glm::vec4 m_SquareColor;
};

