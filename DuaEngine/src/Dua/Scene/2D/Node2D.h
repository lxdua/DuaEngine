#pragma once

#include "Dua/Scene/Main/CanvasItem.h"

namespace Dua {

	class Node2D : public CanvasItem
	{
	private:
		glm::vec2 Position;
		float Rotation = 0.0;
		glm::vec2 Scale;

		glm::mat3 Transform;

	};

}