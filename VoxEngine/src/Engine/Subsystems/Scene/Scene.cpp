#include "voxpch.h"
#include "Scene.h"

namespace vox
{
	entt::registry* Entity::m_Registry = nullptr;

	Entity::Entity(entt::entity id, entt::registry* registry) :
		m_Id(id)
	{
		if (!m_Registry)
		{
			m_Registry = registry;
		}
	}

	Entity Scene::createEntity()
	{
		return { m_Registry.create(), &m_Registry };
	}
}