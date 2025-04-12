#include "duapch.h"
#include "Entity.h"

namespace Dua {

	Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene)
	{
	}

}