#pragma once

#include "Node2D.h"
#include "Dua/Renderer/Texture.h"

namespace Dua {

	class Sprite2D : public Node2D
	{
	protected:

		Ref<Texture2D> texture;
		glm::vec4 modulate = glm::vec4(1.0);

	public:
		virtual void draw() override;

	};

}