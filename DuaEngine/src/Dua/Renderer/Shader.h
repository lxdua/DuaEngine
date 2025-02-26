#pragma once

#include <string>

namespace Dua {

	class Shader
	{
	public:
		Shader(const std::string& vertexSource, std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};

}