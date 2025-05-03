#pragma once

#include "Dua/Core/Memory.h"
#include "ImGui/imgui.h"
#include <string>
#include <functional>

namespace Dua {

	class Scene;
	class Entity;

	class EditorLayer;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(EditorLayer* editorLayer);

		void OnDraw();

	private:
		void DrawEntityNode(Ref<Entity> entity);
		void DrawComponent(Ref<Entity> entity);
		void DrawComponentRemoveButton(const std::string& name, const std::function<void()>& removeCallback);

	private:
		friend class Scene;

		EditorLayer* m_EditorLayer;

		bool m_ShowAddComponentPopup = false;
		ImVec2 m_ComponentWindowSize = ImVec2(400, 500);
		std::string m_HoveredComponent;
	};

}