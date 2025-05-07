#include "duapch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "System.h"

#include "glm/glm.hpp"
#include "Dua/Renderer/Renderer2D.h"

namespace Dua {

	Ref<Entity> Scene::CreateEntity(const std::string name)
	{
		Ref<Entity> entity = CreateRef<Entity>(m_Registry.create(), this);
		m_EntityMap.emplace(entity->m_EntityHandle, entity);

		entity->AddComponent<TransformComponent>();
		auto& tagc = entity->AddComponent<TagComponent>();
		tagc.Tag = name;
		return entity;
	}

	void Scene::DestroyEntity(Ref<Entity> entity)
	{
		m_Registry.destroy(entity->m_EntityHandle);
		m_EntityMap.erase(entity->m_EntityHandle);
		entity->m_EntityHandle = entt::null;
		entity->m_Scene = nullptr;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//NativeScriptSystem::InitScripts(m_Registry, *this);
		//NativeScriptSystem::UpdateScripts(m_Registry, *this, ts);
		TransformSystem::UpdateTransforms(m_Registry);

		auto view = m_Registry.view<TransformComponent, SpriteComponent>();
		for (auto [entity, transform, sprite] : view.each())
		{
			if (sprite.Show)
			{
				Renderer2D::DrawQuad(transform.Transform, sprite.Texture, sprite.Modulate);
			}
		}
	}

	void Scene::OnDestory()
	{
		//NativeScriptSystem::DestroyScripts(m_Registry, *this);
	}

	void Scene::OnRuntimeStart()
	{
		PhysicsSystem::PhysicsStart(m_Registry, m_PhysicsWorldId);
	}

	void Scene::OnRuntimeUpdate(Timestep ts)
	{
		LuaScriptSystem::UpdateScripts(m_Registry, *this, ts);

		PhysicsSystem::PhysicsUpdate(m_Registry, m_PhysicsWorldId, ts);

		TransformSystem::UpdateTransforms(m_Registry);
		SpriteSystem::DrawSprite(m_Registry);

		std::cout << "Update" << std::endl;
	}

	void Scene::OnRuntimeStop()
	{
		b2DestroyWorld(m_PhysicsWorldId);
	}

}

