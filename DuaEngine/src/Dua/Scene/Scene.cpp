#include "duapch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "glm/glm.hpp"

namespace Dua {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{

	}

}