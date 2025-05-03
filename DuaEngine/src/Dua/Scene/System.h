#pragma once

#include "entt/entt.hpp"
#include "Components.h"
#include "Dua/LuaScript/LuaScriptComponent.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Dua {

    class NativeScriptSystem {
    public:
        // ��ʼ������ӵĽű�
        static void InitScripts(entt::registry& registry, Scene& scene) {
            auto view = registry.view<NativeScriptComponent>();
            for (auto [enttEntity, script] : view.each()) {
                // ʹ�����Entity���캯��
                Entity entity(enttEntity, &scene);
                if (script.instance && script.OnCreate && !script.initialized) {
                    script.OnCreate(script.instance.get(), entity);
                    script.initialized = true;
                }
            }
        }
        // ÿ֡���½ű�
        static void UpdateScripts(entt::registry& registry, Scene& scene, Timestep ts) {
            auto view = registry.view<NativeScriptComponent>();
            for (auto [enttEntity, script] : view.each()) {
                // ��ȷ����Entityʵ��
                Entity entity(enttEntity, &scene);
                if (script.instance && script.OnUpdate) {
                    script.OnUpdate(script.instance.get(), entity, ts);
                }
            }
        }
        // ���ٽű�
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
        // �������д����ǵı任���
        static void UpdateTransforms(entt::registry& registry)
        {
            auto view = registry.view<TransformComponent>();
            for (auto entity : view)
            {
                auto& transform = view.get<TransformComponent>(entity);
                if (transform.Dirty)
                {
                    UpdateTransform(transform);
                    transform.Dirty = false; // �������
                }
            }
        }

        // ǿ�Ƹ���ĳ��ʵ��ı任����ʹû�����ǣ�
        static void ForceUpdateTransform(entt::registry& registry, entt::entity entity)
        {
            auto& transform = registry.get<TransformComponent>(entity);
            UpdateTransform(transform);
            transform.Dirty = false;
        }

    private:
        // ʵ�ʼ���任������߼�
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

}