#include "duapch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "System.h"

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
		TransformSystem::UpdateTransforms(m_Registry);

		auto view = m_Registry.view<TransformComponent, SpriteComponent>();
		for (auto [entity, transform, sprite] : view.each())
		{
			transform.SetPosition(transform.Position + glm::vec3(ts, 0, 0));
			Renderer2D::DrawQuad(transform.Transform, sprite.Texture, sprite.Modulate);
		}
	}

}

