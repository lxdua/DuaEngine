#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <iostream>

#include "SparseSets.h"

namespace Dua::ecs {

	using EntityID = uint32_t;

    struct Component{};
    struct Resource{};

    template<typename Category>
    class IndexGetter final
    {
    public:
        template<typename T>
        static uint32_t Get()
        {
            static uint32_t id = cur_id++;
            return id;
        }
    private:
        inline static std::atomic<uint32_t> cur_id = 0;
    };

    template<typename T>
    struct IDGenerator final
    {
    public:
        static T Gen()
        {
            return cur_id++;
        }
    private:
        inline static std::atomic<T> cur_id = {};
    };

    class World final
    {
    public:
        friend class Commands;

    private:

        struct Pool final
        {
            std::vector<void*> instances;
            std::vector<void*> cache;

            using CreateFunc = void* (*)(void);
            using DestroyFunc = void(*)(void*);

            CreateFunc create;
            DestroyFunc destroy;

            Pool(CreateFunc create, DestroyFunc destroy) : create(create), destroy(destroy) {}

            void* Create()
            {
                if (!cache.empty())
                {
                    instances.push_back(cache.back());
                    cache.pop_back();
                }
                else
                {
                    instances.push_back(create());
                }
                return instances.back();
            }

            void Destroy(void* elem)
            {
                auto it = std::find(instances.begin(), instances.end(), elem);
                if (it != instances.end())
                {
                    cache.push_back(*it);
                    std::swap(*it, instances.back());
                    instances.pop_back();
                }
                else
                {
                    std::cout << "Your elem not in pool" << std::endl;
                }
            }
        };

        struct ComponentInfo // 跟踪哪些Entity拥有这个组件
        {
            Pool pool;
            SparseSets<EntityID, 32> sparse_set;

            ComponentInfo(Pool::CreateFunc create, Pool::DestroyFunc destroy)
                : pool(create, destroy) { }
            ComponentInfo()
                : pool(nullptr, nullptr) { }
        };

        using ComponentMap = std::unordered_map<uint32_t, ComponentInfo>;
        ComponentMap component_map;
        using ComponentContainer = std::unordered_map<uint32_t, void*>;
        std::unordered_map<EntityID, ComponentContainer> entities;

        struct ResourceInfo
        {
            void* resource = nullptr;

            using CreateFunc = void* (*)(void);
            using DestroyFunc = void(*)(void*);

            CreateFunc create;
            DestroyFunc destroy;
        };

    };


    class Commands final
    {
    public:
        Commands(World& world) : world(world) {}

        template<typename... ComponentTypes>
        Commands& Spawn(ComponentTypes&&... components)
        {
            EntityID entity_id = IDGenerator<EntityID>::Gen();
            DoSpawn(entity_id, std::forward<ComponentTypes>(components)...); // 递归添加组件
            return *this;
        }

        Commands& Destroy(EntityID entity_id)
        {
            if (auto it = world.entities.find(entity_id); it != world.entities.end())
            {
                for (auto [id, component] : it->second)
                {
                    auto& component_info = world.component_map[id];
                    component_info.pool.Destroy(component);
                    component_info.sparse_set.Remove(entity_id);
                }
                world.entities.erase(it);
            }
            return *this;
        }

        template<typename T>
        Commands& SetResource(T&& resource)
        {
            auto index = IndexGetter<Resource>::Get<T>();
            if (auto it = world.resources.find(index); it != world.resources.end())
            {
                std::cout << "resource already exists" << std::endl;
            }
            else
            {
                auto new_it = world.resources.emplace(
                    index,
                    World::ResourceInfo()
                );
                new_it.first->second.resource = new T;
                *(T*)(new_it.first->second.resource) = std::forward<T>(resource);
            }
            return *this;
        }

        template<typename T>
        Commands& RemoveResource(T&& resource)
        {
            auto index = IndexGetter<Resource>::Get<T>();
            if (auto it = world.resources.find(index); it == world.resources.end())
            {
                delete (T*)it->second.resource;
            }
            return *this;
        }

    private:
        World& world;

        template<typename T, typename... Remains>
        void DoSpawn(EntityID entity_id, T&& component, Remains&&... remains)
        {
            auto index = IndexGetter::Get<T>();
            if (auto it = world.component_map.find(index); it == world.component_map.end())
            {
                world.component_map.emplace(
                    index,
                    World::ComponentInfo(
                        []()->void* { return new T; },
                        [](void* elem) { delete (T*)(elem); }
                    );
                );
            }
            World::ComponentInfo& component_info = world.component_map[index];
            void* elem = component_info.pool.Create();
            *((T*)elem) = std::forward<T>(component);
            component_info.sparse_set.Add(entity_id);

            auto it = world.entities.emplace(entity_id);
            it.first->second[index] = elem;

            if constexpr (sizeof...(remains) != 0)
            {
                DoSpawn<Remains...>(entity_id, std::forward<Remains>(remains)...);
            }
        }


    };

}