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
		b2WorldDef m_Box2DWorldDef = b2DefaultWorldDef();
		m_Box2DWorldDef.gravity = b2Vec2({ 0.0f, -10.0f });

		m_PhysicsWorldId = b2CreateWorld(&m_Box2DWorldDef);

		auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
		for (auto &[entity, transform, rigidbody2d] : view.each())
		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.position = b2Vec2({ transform.Position.x, transform.Position.y});
			bodyDef.rotation = b2MakeRot(transform.Rotation);
			bodyDef.fixedRotation = rigidbody2d.FixedRotation;
			bodyDef.type = b2BodyType::b2_dynamicBody;

			rigidbody2d.BodyId = b2CreateBody(m_PhysicsWorldId, &bodyDef);

			b2Polygon polygon = rigidbody2d.Polygon;
			b2ShapeDef shapeDef = b2DefaultShapeDef();
			b2CreatePolygonShape(rigidbody2d.BodyId, &shapeDef, &polygon);
		}

	}

	void Scene::OnRuntimeUpdate(Timestep ts)
	{
		LuaScriptSystem::UpdateScripts(m_Registry, *this, ts);
		TransformSystem::UpdateTransforms(m_Registry);

		// Physics
		PhysicsSystem::UpdatePhysics(m_Registry, m_PhysicsWorldId, ts);

		// Render
		SpriteSystem::DrawSprite(m_Registry);
	}

	void Scene::OnRuntimeStop()
	{
		b2DestroyWorld(m_PhysicsWorldId);
	}

}

