#include "duapch.h"
#include "OpenGLShader.h"

#include "glm/gtc/type_ptr.hpp"

#include <fstream>

namespace Dua {

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	void OpenGLShader::SetInt(const std::string& name, const int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, const int value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3 value)
	{
		UploadUniformVec3(name, value);
	}

	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4 value)
	{
		UploadUniformVec4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4 value)
	{
		UploadUniformMat4(name, value);
	}


	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else
		{
			std::cout << "Could not open file: " << filepath << std::endl;
		}
		return result;
	}

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		return NULL;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			//err
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			//err
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(
				nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)
			);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSource)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs; // shaderSource.size() <= 2
		int glShaderIDIndex = 0;
		for (auto& [type, src] : shaderSource)
		{
			GLuint shader = glCreateShader(type);

			const GLchar* source = (const GLchar*)src.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				std::cout << "Shader compilation failure!" << std::endl;

				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			
			for (auto& id : glShaderIDs)
				glDeleteShader(id);
			
			std::cout << "Shader link failure!" << std::endl;

			return;
		}

		for (auto& id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}


	void OpenGLShader::UploadUniformInt(const std::string& name, int value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformBool(const std::string& name, bool value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value ? 1 : 0);
	}

	void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformIVec2(const std::string& name, const glm::ivec2& value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformIVec3(const std::string& name, const glm::ivec3& value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformIVec4(const std::string& name, const glm::ivec4& value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& matrix) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformSampler2D(const std::string& name, int textureSlot) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, textureSlot);
	}

	void OpenGLShader::UploadUniformSamplerCube(const std::string& name, int textureSlot) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, textureSlot);
	}

	void OpenGLShader::UploadUniformFloatArray(const std::string& name, float* values, uint32_t count) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1fv(location, count, values);
	}




}