#pragma once

#include <string>
#include "glm/glm.hpp"
#include "Dua/Renderer/Camera.h"
#include "Dua/Renderer/Texture.h"

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
		using ScriptPtr = std::unique_ptr<void, void(*)(void*)>;

		// 生命周期函数指针
		using InitFunc = void(*)(void*, Entity);
		using UpdateFunc = void(*)(void*, Entity, float);
		using DestroyFunc = void(*)(void*, Entity);

		ScriptPtr instance{ nullptr, [](void*) {} };
		InitFunc OnCreate = nullptr;
		UpdateFunc OnUpdate = nullptr;
		DestroyFunc OnDestroy = nullptr;
		bool initialized = false;

		template<typename T>
		void Bind() 
		{
			static_assert(
				std::is_invocable_r_v<void, decltype(&T::OnCreate), T*, Entity> &&
				std::is_invocable_r_v<void, decltype(&T::OnUpdate), T*, Entity, float> &&
				std::is_invocable_r_v<void, decltype(&T::OnDestroy), T*, Entity>,
				"Script must implement OnCreate, OnUpdate and OnDestroy"
				);

			// 自定义删除器
			auto deleter = [](void* ptr) { delete static_cast<T*>(ptr); };
			instance = ScriptPtr(new T(), deleter);

			// 绑定函数
			OnCreate = [](void* ptr, Entity e) { static_cast<T*>(ptr)->OnCreate(e); };
			OnUpdate = [](void* ptr, Entity e, float dt) { static_cast<T*>(ptr)->OnUpdate(e, dt); };
			OnDestroy = [](void* ptr, Entity e) { static_cast<T*>(ptr)->OnDestroy(e); };
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