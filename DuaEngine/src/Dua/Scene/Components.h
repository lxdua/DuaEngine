#pragma once

#include <string>
#include "glm/glm.hpp"
#include "Dua/Renderer/Camera.h"
#include "Dua/Renderer/Texture.h"
#include "ScriptableEntity.h"

namespace Dua {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [this]() { Instance = new T(); };
			DestroyInstanceFunction = [&Instance]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(); };
		}
	};

	struct TransformComponent
	{
		glm::vec3 Position{ 0.0f };
		float Rotation = 0.0f;
		glm::vec3 Scale{ 1.0f };
		glm::mat4 Transform{ 1.0f };
		bool Dirty = true;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		void SetPosition(const glm::vec3& pos)
		{
			if (Position != pos)
			{
				Position = pos;
				Dirty = true;
			}
		}
		void SetRotation(float radians)
		{
			if (Rotation != radians)
			{
				Rotation = radians;
				Dirty = true;
			}
		}
		void SetScale(const glm::vec3& scale)
		{
			if (Scale != scale)
			{
				Scale = scale;
				Dirty = true;
			}
		}
	};

	struct SpriteComponent
	{
		Ref<Texture2D> Texture = Texture2D::Create("Assets/Textures/ntx.png");
		glm::vec4 Modulate = glm::vec4(1.0);

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const Ref<Texture2D> texture) : Texture(texture) {}
		SpriteComponent(const Ref<Texture2D> texture, const glm::vec4 modulate) : Texture(texture), Modulate(modulate) {}
	};

	struct ColorRectComponent
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		ColorRectComponent() = default;
		ColorRectComponent(const ColorRectComponent&) = default;
		ColorRectComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent
	{
		Dua::Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection) : Camera(projection) {}
	};

}