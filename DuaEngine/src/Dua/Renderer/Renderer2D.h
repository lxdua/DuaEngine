#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Dua {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4 modulate, bool keepAspectRatio = true);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4 modulate, bool keepAspectRatio = true);

		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, glm::vec4 modulate, bool keepAspectRatio = true);

	private:
		static void FlushAndReset();

	};

}