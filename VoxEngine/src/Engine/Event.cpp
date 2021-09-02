#include "voxpch.h"
#include "Event.h"

namespace vox
{
	MouseMoveEvent::MouseMoveEvent(int dx, int dy) :
		m_Dx(dx),
		m_Dy(dy)
	{

	}

	int MouseMoveEvent::getDx() const
	{
		return m_Dx;
	}

	int MouseMoveEvent::getDy() const
	{
		return m_Dy;
	}

	KeyEvent::KeyEvent(int key) :
		m_Key(key),
		m_Action(0)
	{

	}

	KeyEvent::KeyEvent(int key, int action) :
		m_Key(key),
		m_Action(action)
	{

	}

	int KeyEvent::getKey() const
	{
		return m_Key;
	}

	int KeyEvent::getAction() const
	{
		return m_Action;
	}

}
