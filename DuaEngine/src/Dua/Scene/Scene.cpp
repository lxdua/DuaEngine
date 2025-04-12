#include "duapch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "glm/glm.hpp"
#include "Dua/Renderer/Renderer2D.h"

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
		auto group = m_Registry.group<TransformComponent>(entt::get<ColorRectComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, ColorRectComponent>(entity);
			//Renderer2D::DrawQuad()
		}
	}

}