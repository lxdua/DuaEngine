#include "duapch.h"
#include "SceneSerializer.h"
#include "Dua/Scene/Entity.h"
#include "Dua/Scene/Components.h"
#include "glm/gtc/matrix_transform.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>

namespace Dua {

	SceneSerializer::SceneSerializer(const Ref<Scene> scene) : m_Scene(scene)
	{
	}

    static void SerializeEntity(YAML::Emitter& out, Entity& entity)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<TagComponent>().Tag;

        // 序列化组件
        out << YAML::Key << "Components" << YAML::Value << YAML::BeginMap;
        
        // TransformComponent
        if (entity.HasComponent<TransformComponent>())
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "TransformComponent" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "Position" << YAML::Flow << YAML::BeginSeq
                << transform.Position.x << transform.Position.y << transform.Position.z << YAML::EndSeq;
            out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
            out << YAML::Key << "Scale" << YAML::Flow << YAML::BeginSeq
                << transform.Scale.x << transform.Scale.y << transform.Scale.z << YAML::EndSeq;
            out << YAML::EndMap;
        }

        // SpriteComponent
        if (entity.HasComponent<SpriteComponent>())
        {
            auto& sprite = entity.GetComponent<SpriteComponent>();
            out << YAML::Key << "SpriteComponent" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "Visible" << YAML::Value << sprite.Show;
            out << YAML::Key << "Texture" << YAML::Value << sprite.Texture->GetPath();
            out << YAML::Key << "Modulate" << YAML::Flow << YAML::BeginSeq
                << sprite.Modulate.r << sprite.Modulate.g
                << sprite.Modulate.b << sprite.Modulate.a << YAML::EndSeq;
            out << YAML::EndMap;
        }
        
        out << YAML::EndMap; // Components结束
        out << YAML::EndMap; // Entity结束
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        auto& registry = m_Scene->m_Registry;

        std::cout << "Start serialize" << std::endl;

        // 遍历所有拥有TagComponent的实体
        auto view = registry.view<TagComponent>();
        for (auto entityID : view)
        {
            Entity entity{ entityID, m_Scene.get() };
            std::cout << "Serialize " << (uint32_t)entityID << std::endl;
            if (!entity.IsValid())
                return;
            SerializeEntity(out, entity);
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string & filepath)
    {

    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        std::ifstream stream(filepath);
        YAML::Node data = YAML::Load(stream);

        if (!data["Scene"]) return false;

        auto entities = data["Entities"];
        if (entities) 
        {
            for (auto entityNode : entities) 
            {
                std::string name = entityNode["Entity"].as<std::string>();
                Entity entity = m_Scene->CreateEntity(name);

                auto components = entityNode["Components"];
                if (components) 
                {
                    // TransformComponent
                    if (auto transformNode = components["TransformComponent"]) 
                    {
                        auto& transform = entity.GetComponent<TransformComponent>();

                        auto position = transformNode["Position"].as<std::vector<float>>();
                        transform.Position = { position[0], position[1], position[2] };

                        transform.Rotation = transformNode["Rotation"].as<float>();

                        auto scale = transformNode["Scale"].as<std::vector<float>>();
                        transform.Scale = { scale[0], scale[1], scale[2] };

                        // 直接重建变换矩阵
                        transform.Transform = glm::translate(glm::mat4(1.0f), transform.Position)
                            * glm::rotate(glm::mat4(1.0f), transform.Rotation, { 0, 0, 1 })
                            * glm::scale(glm::mat4(1.0f), transform.Scale);
                        transform.Dirty = false;
                    }

                    // SpriteComponent
                    if (auto spriteNode = components["SpriteComponent"]) 
                    {
                        auto& sprite = entity.AddComponent<SpriteComponent>();

                        sprite.Show = spriteNode["Visible"].as<bool>();

                        std::string texturePath = spriteNode["Texture"].as<std::string>();
                        sprite.Texture = Texture2D::Create(texturePath);

                        auto color = spriteNode["Modulate"].as<std::vector<float>>();
                        sprite.Modulate = { color[0], color[1], color[2], color[3] };
                    }
                }
            }
        }
        return true;
    }

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return false;
	}

}