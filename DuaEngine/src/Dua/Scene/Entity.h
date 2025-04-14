#pragma once

#include "entt/entt.hpp"
#include <cassert>
#include <unordered_map>

namespace Dua {

    class Scene;

    class Entity
    {
    public:

        inline static std::unordered_map<entt::entity, Ref<Entity>> EntityMap;

        Entity(entt::entity handle, Scene* scene)
            : m_EntityHandle(handle), m_Scene(scene)
        {
            // 确保实体和场景有效
            assert(handle != entt::null && "Entity handle cannot be null");
            assert(scene != nullptr && "Scene pointer cannot be null");
        }

        Entity(const Entity& other) = default;

        Entity(const Ref<Entity>& other)
        {
            m_EntityHandle = other->m_EntityHandle;
            m_Scene = other->m_Scene;
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            // 前置条件检查
            assert(IsValid() && "Cannot add component to invalid entity");
            assert(m_Scene != nullptr && "Scene pointer is null");
            assert(!HasComponent<T>() && "Component already exists");

            return m_Scene->m_Registry.template emplace<T>(
                m_EntityHandle,
                std::forward<Args>(args)...
            );
        }

        template<typename T>
        T& GetComponent()
        {
            // 前置条件检查
            assert(IsValid() && "Cannot get component from invalid entity");
            assert(m_Scene != nullptr && "Scene pointer is null");
            assert(HasComponent<T>() && "Component does not exist");

            return m_Scene->m_Registry.template get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent() const
        {
            // 基本有效性检查
            assert(IsValid() && "Cannot check component on invalid entity");
            assert(m_Scene != nullptr && "Scene pointer is null");

            return m_Scene->m_Registry.template any_of<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            // 前置条件检查
            assert(IsValid() && "Cannot remove component from invalid entity");
            assert(m_Scene != nullptr && "Scene pointer is null");
            assert(HasComponent<T>() && "Component does not exist");

            m_Scene->m_Registry.template remove<T>(m_EntityHandle);
        }

        uint32_t GetID() const
        {
            return (uint32_t)m_EntityHandle;
        }

        bool operator==(const Entity& other) const
        {
            return m_EntityHandle == other.m_EntityHandle;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

        // 显式有效性检查方法
        bool IsValid() const
        {
            return m_EntityHandle != entt::null && m_Scene != nullptr;
        }

    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene = nullptr;

        friend class Scene;
    };

}