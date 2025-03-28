#include "duapch.h"
#include "Sprite2D.h"

namespace Dua {

	void Sprite2D::draw()
	{
		Renderer2D::DrawQuad(position, scale, rotation, texture, modulate);
	}

}