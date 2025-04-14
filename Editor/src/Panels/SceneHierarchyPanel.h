#pragma once

#include "Dua/Core/Memory.h"

namespace Dua {

	class Scene;
	class Entity;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Ref<Entity> entity);
		void DrawComponent(Ref<Entity> entity);

	private:
		Ref<Scene> m_Context;
		Ref<Entity> m_SelectedEntity;
		friend class Scene;
	};

}