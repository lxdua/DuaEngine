#pragma once

#include "Dua.h"
#include "OrthographicCameraController.h"

#include "UI/TopMenu.h"
#include "UI/SceneHierarchyPanel.h"
#include "UI/ViewportPanel.h"
#include "UI/ContentBrowserPanel.h"

#include <filesystem>

namespace Dua {

	enum class EditorRuntimeState
	{
		Editing,    // 编辑模式
		Running,    // 运行模式
		//Paused      // 暂停模式
	};

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

		void OpenFile(const std::filesystem::path& path);

		void ToggleRuntimeState(EditorRuntimeState state);

	private:

		Ref<Scene> m_Scene;
		Ref<Scene> m_BackupScene;

		OrthographicCameraController m_CameraController;
		ShaderLibrary m_ShaderLib;

		glm::vec2 m_ViewportSize = { 1280.0f,720.0f };

		Ref<Framebuffer> m_Framebuffer;

		EditorRuntimeState m_RuntimeState;

		// UI
		Ref<TopMenu> m_TopMenu;
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Ref<ViewportPanel> m_ViewportPanel;
		Ref<ContentBrowserPanel> m_ContentBrowserPanel;

		Ref<Entity> m_SelectedEntity;

		friend class TopMenu;
		friend class SceneHierarchyPanel;
		friend class ViewportPanel;
		friend class ContentBrowserPanel;

	};

}