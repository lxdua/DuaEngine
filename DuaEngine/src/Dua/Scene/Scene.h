#pragma once

#include "entt/entt.hpp"
#include "Dua/Core/Timestep.h"

namespace Dua {

	class Entity;
	class SceneHierarchyPanel;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string name = "");

		void OnUpdate(Timestep ts);
		void OnDestory();

	private:
		entt::registry m_Registry;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}