#pragma once

#include "entt/entt.hpp"
#include "Dua/Core/Timestep.h"

namespace Dua {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string name);

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
		friend class Entity;
	};

}