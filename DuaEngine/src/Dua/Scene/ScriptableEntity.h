#pragma once

#include "Entity.h"

namespace Dua {

	class ScriptableEntity
	{
	public:
		//template<typename T>

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};

}