#pragma once

#include "Dua/Renderer/Shader.h"

#include "GLAD/glad.h"
#include "GLM/glm.hpp"

namespace Dua {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformFloat(const std::string& name, float value);

        void UploadUniformBool(const std::string& name, bool value);

        void UploadUniformVec2(const std::string& name, const glm::vec2& value);
        void UploadUniformVec3(const std::string& name, const glm::vec3& value);
        void UploadUniformVec4(const std::string& name, const glm::vec4& value);

        void UploadUniformIVec2(const std::string& name, const glm::ivec2& value);
        void UploadUniformIVec3(const std::string& name, const glm::ivec3& value);
        void UploadUniformIVec4(const std::string& name, const glm::ivec4& value);

        void UploadUniformMat2(const std::string& name, const glm::mat2& matrix);
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

        void UploadUniformSampler2D(const std::string& name, int textureSlot);
        void UploadUniformSamplerCube(const std::string& name, int textureSlot);

        void UploadUniformFloatArray(const std::string& name, float* values, uint32_t count);

	private:
		uint32_t m_RendererID;

        //std::unordered_map<std::string, GLint> m_UniformLocationCache;
        //GLint GetUniformLocation(const std::string& name);

	};

}