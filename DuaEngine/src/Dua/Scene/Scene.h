#pragma once

#include "entt/entt.hpp"
#include "Dua/Core/Timestep.h"
#include "box2d/box2d.h"

namespace Dua {

	class Entity;
	class Component;

	class SceneHierarchyPanel;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Ref<Entity> CreateEntity(const std::string name = "");
		void DestroyEntity(Ref<Entity> entity);

		void OnUpdate(Timestep ts);
		void OnDestory();

		void OnRuntimeStart();
		void OnRuntimeUpdate(Timestep ts);
		void OnRuntimeStop();

		entt::registry m_Registry;
		std::unordered_map<entt::entity, Ref<Entity>> m_EntityMap;

		b2WorldId m_PhysicsWorldId;
	};

}