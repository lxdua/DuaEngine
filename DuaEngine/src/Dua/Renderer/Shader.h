#pragma once

#include "Dua/Core/Memory.h"
#include "glm/glm.hpp"

#include <string>
#include <unordered_map>

namespace Dua {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, std::string& fragmentSource);

		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, const int value) = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3 value) = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4 value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4 value) = 0;

	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, std::string& filepath);
		Ref<Shader> Get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}