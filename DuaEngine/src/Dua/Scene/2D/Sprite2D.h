#pragma once

#include "Node2D.h"
#include "Dua/Renderer/Texture.h"

namespace Dua {

	class Sprite2D : public Node2D
	{
	private:

		Ref<Texture2D> texture;

	public:
		virtual void draw() override;

	};

}