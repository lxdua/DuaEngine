#pragma once

#include "Dua/Core/Memory.h"
#include "Dua/Scene/Scene.h"

namespace Dua {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_Context;
		uint32_t m_SelectedEntityID;
		friend class Scene;
	};

}