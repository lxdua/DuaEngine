#pragma once

#include "entt/entt.hpp"
#include "Components.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Dua {

    class NativeScriptSystem
    {
    public:
        static void UpdateNativeScriptComponent(entt::registry& registry)
        {
            auto view = registry.view<NativeScriptComponent>();
            view.each(
                [](const auto entity, const auto& nsc)
                {
                    if (!nsc.Instance)
                    {
                        nsc.InstantiateFunction();
                    }
                    //nsc.OnUpdateFunction(nsc.Instance, ts);
                }
            );
        }
    };

    class TransformSystem
    {
    public:
        // 更新所有带脏标记的变换组件
        static void UpdateTransforms(entt::registry& registry)
        {
            auto view = registry.view<TransformComponent>();
            for (auto entity : view)
            {
                auto& transform = view.get<TransformComponent>(entity);
                if (transform.Dirty)
                {
                    UpdateTransform(transform);
                    transform.Dirty = false; // 清除脏标记
                }
            }
        }

        // 强制更新某个实体的变换（即使没有脏标记）
        static void ForceUpdateTransform(entt::registry& registry, entt::entity entity)
        {
            auto& transform = registry.get<TransformComponent>(entity);
            UpdateTransform(transform);
            transform.Dirty = false;
        }

    private:
        // 实际计算变换矩阵的逻辑
        static void UpdateTransform(TransformComponent& transform)
        {
            const glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.Position);
            const glm::mat4 rotation = glm::rotate(
                glm::mat4(1.0f),
                transform.Rotation,
                glm::vec3(0.0f, 0.0f, 1.0f) // 绕 Z 轴旋转（2D）
            );
            const glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.Scale);
            transform.Transform = translation * rotation * scale;
        }
    };

}