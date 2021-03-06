#include "voxpch.h"
#include "Subsystem.h"

#include "Archsystem.h"

namespace vox
{
	Subsystem::Subsystem(Archsystem* parent) : m_Parent(parent)
	{
	}

	Subsystem::Subsystem() :
		m_Parent(nullptr)
	{
	}

	void Subsystem::setParent(Archsystem* parent)
	{
		m_Parent = parent;
	}

	Subsystem* Subsystem::getSub(SubType type)
	{
		return m_Parent->getSub(type);
	}

	Renderer* Subsystem::getRenderer()
	{
		return m_Parent->getRenderer();
	}

	Scene* Subsystem::getScene()
	{
		return m_Parent->getScene();
	}

	EventHandler* Subsystem::getEventHandler()
	{
		return m_Parent->getEventHandler();
	}

}
