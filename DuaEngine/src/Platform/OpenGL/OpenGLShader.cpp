#include "duapch.h"
#include "OpenGLShader.h"

#include "glm/gtc/type_ptr.hpp"

namespace Dua {

	OpenGLShader::OpenGLShader(const std::string& vertexSource, std::string& fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			std::cout << "Vertex shader compilation failure!" << std::endl;

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			std::cout << "Fragment shader compilation failure!" << std::endl;

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			std::cout << "Shader link failure!" << std::endl;

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		m_RendererID = program;
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

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) 
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
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