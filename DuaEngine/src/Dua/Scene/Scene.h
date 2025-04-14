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

		Ref<Entity> CreateEntity(const std::string name = "");
		void DestroyEntity(Ref<Entity> entity);

		void OnUpdate(Timestep ts);
		void OnDestory();

		entt::registry m_Registry;
		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}