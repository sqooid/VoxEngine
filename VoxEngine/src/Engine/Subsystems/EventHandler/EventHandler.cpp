#include "voxpch.h"
#include "EventHandler.h"

namespace vox
{
	void EventHandler::onAttach()
	{
	}



	void* EventHandler::getCallbackList(EventType type)
	{
		switch (type)
		{
		case EventType::Key:
			return static_cast<void*>(&m_KeyEventCallbacks);
			break;
		case EventType::MouseMove:
			return static_cast<void*>(&m_MouseMoveEventCallbacks);
			break;
		case EventType::MouseClick:
			return static_cast<void*>(&m_MouseClickEventCallbacks);
			break;
		default:
			break;
		}

	}

}


