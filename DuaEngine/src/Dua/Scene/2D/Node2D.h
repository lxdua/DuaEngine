#pragma once

#include "Dua/Scene/Main/CanvasItem.h"

namespace Dua {

	class Node2D : public CanvasItem
	{
	private:
		glm::vec2 position;
		float rotation = 0.0;
		glm::vec2 scale;

	public:
		inline glm::vec2 get_position() const { return position; }
		inline float get_rotation() const { return rotation; }
		inline glm::vec2 get_scale() const { return scale; }

		inline void set_position(const glm::vec2& value) { position = value; }
		inline void set_rotation(const float &value) { rotation = value; }
		inline void set_scale(const glm::vec2 &value) { scale = value; }

	};

}