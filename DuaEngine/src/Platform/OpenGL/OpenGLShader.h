#pragma once

#include "Dua/Renderer/Shader.h"

#include "GLAD/glad.h"
#include "glm/glm.hpp"

namespace Dua {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

        virtual const std::string& GetName() const override;

        virtual void SetInt(const std::string& name, const int value) override;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
        virtual void SetFloat(const std::string& name, const int value) override;
        virtual void SetVec3(const std::string& name, const glm::vec3 value) override;
        virtual void SetVec4(const std::string& name, const glm::vec4 value) override;
        virtual void SetMat4(const std::string& name, const glm::mat4 value) override;


        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
        
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
        std::string m_Name;

        //std::unordered_map<std::string, GLint> m_UniformLocationCache;
        //GLint GetUniformLocation(const std::string& name);

        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(std::unordered_map<GLenum, std::string>& shaderSource);

	};

}