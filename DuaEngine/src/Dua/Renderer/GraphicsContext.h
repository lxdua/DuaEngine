#pragma once

#include "Dua/Core.h"

namespace Dua {

	class DUA_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};

}