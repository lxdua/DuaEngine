#pragma once

#include "Dua/Renderer/Renderer2D.h"
#include "entt/entt.hpp"
#include "Components.h"
#include "Dua/LuaScript/LuaScriptComponent.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Dua {

    class NativeScriptSystem {
    public:
        // 初始化新添加的脚本
        static void InitScripts(entt::registry& registry, Scene& scene) {
            auto view = registry.view<NativeScriptComponent>();
            for (auto [enttEntity, script] : view.each()) {
                // 使用你的Entity构造函数
                Entity entity(enttEntity, &scene);
                if (script.instance && script.OnCreate && !script.initialized) {
                    script.OnCreate(script.instance.get(), entity);
                    script.initialized = true;
                }
            }
        }
        // 每帧更新脚本
        static void UpdateScripts(entt::registry& registry, Scene& scene, Timestep ts) {
            auto view = registry.view<NativeScriptComponent>();
            for (auto [enttEntity, script] : view.each()) {
                // 正确构造Entity实例
                Entity entity(enttEntity, &scene);
                if (script.instance && script.OnUpdate) {
                    script.OnUpdate(script.instance.get(), entity, ts);
                }
            }
        }
        // 销毁脚本
        static void DestroyScripts(entt::registry& registry, Scene& scene) {
            auto view = registry.view<NativeScriptComponent>();
            for (auto [enttEntity, script] : view.each()) {
                Entity entity(enttEntity, &scene);
                if (script.instance && script.OnDestroy) {
                    script.OnDestroy(script.instance.get(), entity);
                }
            }
        }
    };

    class LuaScriptSystem
    {
    public:

        static void InitScripts(entt::registry& registry, Scene& context)
        {
            auto view = registry.view<LuaScriptComponent>();
            for (auto [entity, luaScript] : view.each())
            {
                luaScript.Init();
            }
        }

        static void UpdateScripts(entt::registry& registry, Scene& context, Timestep ts)
        {
            auto view = registry.view<LuaScriptComponent>();
            for (auto [entity, luaScript] : view.each())
            {
                luaScript.Update(ts);
            }
        }
    };

    class TransformSystem
    {
    public:
        static void UpdateTransforms(entt::registry& registry)
        {
            auto view = registry.view<TransformComponent>();
            for (auto entity : view)
            {
                auto& transform = view.get<TransformComponent>(entity);
                if (transform.Dirty)
                {
                    UpdateTransform(transform);
                    transform.Dirty = false;
                }
            }
        }

        static void ForceUpdateTransform(entt::registry& registry, entt::entity entity)
        {
            auto& transform = registry.get<TransformComponent>(entity);
            UpdateTransform(transform);
            transform.Dirty = false;
        }

    private:
        static void UpdateTransform(TransformComponent& transform)
        {
            const glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.Position);
            const glm::mat4 rotation = glm::rotate(
                glm::mat4(1.0f),
                transform.Rotation,
                glm::vec3(0.0f, 0.0f, 1.0f)
            );
            const glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.Scale);
            transform.Transform = translation * rotation * scale;
        }
    };

    class SpriteSystem
    {
    public:
        static void DrawSprite(entt::registry& registry)
        {
            for (auto [entity, transform, sprite] :
                registry.view<TransformComponent, SpriteComponent>().each())
            {
                if (sprite.Show)
                {
                    Renderer2D::DrawQuad(transform.Transform, sprite.Texture, sprite.Modulate);
                }
            }
        }
    };

    class PhysicsSystem
    {
    private:
        static inline const double m_PhysicsTimestep = 0.01;
        static inline double m_PhysicsTimeAccumulator = 0.0;

    public:

        static void PhysicsStart(entt::registry& registry, b2WorldId& worldId)
        {
            m_PhysicsTimeAccumulator = 0.0;

            /*----------*/

            b2WorldDef m_Box2DWorldDef = b2DefaultWorldDef();
            m_Box2DWorldDef.gravity = b2Vec2({ 0.0f, -10.0f });

            worldId = b2CreateWorld(&m_Box2DWorldDef);

            auto view = registry.view<TransformComponent, Physicsbody2DComponent>();
            for (auto [entity, transform, physicsbody2d] : view.each())
            {
                b2BodyDef bodyDef = b2DefaultBodyDef();
                bodyDef.position = b2Vec2({ transform.Position.x, transform.Position.y });
                bodyDef.rotation = b2MakeRot(transform.Rotation);
                bodyDef.fixedRotation = physicsbody2d.FixedRotation;

                bodyDef.type = physicsbody2d.BodyType;

                physicsbody2d.BodyId = b2CreateBody(worldId, &bodyDef);

                b2Polygon polygon = physicsbody2d.Polygon;
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                b2CreatePolygonShape(physicsbody2d.BodyId, &shapeDef, &polygon);
            }
        }

        static void PhysicsUpdate(entt::registry& registry, b2WorldId worldId, Timestep ts)
        {
            m_PhysicsTimeAccumulator += ts;

            while (m_PhysicsTimeAccumulator >= m_PhysicsTimestep)
            {
                m_PhysicsTimeAccumulator -= m_PhysicsTimestep;
                std::cout << "PhysicsUpdate" << std::endl;
                /*----------*/

                int subStepCount = 4;
                b2World_Step(worldId, ts, subStepCount);

                for (auto [entity, transform, physicsbody2d] :
                    registry.view<TransformComponent, Physicsbody2DComponent>().each())
                {
                    b2Vec2 position = b2Body_GetPosition(physicsbody2d.BodyId);
                    transform.SetPosition({ position.x, position.y, transform.Position.z });
                    b2Rot rotation = b2Body_GetRotation(physicsbody2d.BodyId);
                    transform.SetRotation(b2Rot_GetAngle(rotation));
                }

            } 
        }
    };
}