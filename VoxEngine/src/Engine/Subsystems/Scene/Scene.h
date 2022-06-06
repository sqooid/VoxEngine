#pragma once

#include "Engine/Subsystem.h"
#include <entt.hpp>

namespace vox
{
	//////////////////////////////////////////////////////////////////////////
	// Entity class //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, entt::registry* registry);

		/**
		 * @brief Constructs and adds component to entity
		 * @tparam T Component type
		 * @tparam ...A
		 * @param ...args Construction arguments
		 * @return Newly constructed component
		*/
		template<typename T, typename... A>
		T& addComponent(A&&... args)
		{
			return m_Registry->emplace<T>(m_Id, std::forward<A>(args)...);
		}

		template<typename... T>
		bool hasAll()
		{
			return m_Registry->all_of<T...>(m_Id);
		}

	private:
		entt::entity m_Id;
		static entt::registry* m_Registry;
	};


	//////////////////////////////////////////////////////////////////////////
	// Scene class ///////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class Scene :
		public Subsystem
	{
	public:
		Entity createEntity();

	private:
		entt::registry m_Registry;
	};
}



