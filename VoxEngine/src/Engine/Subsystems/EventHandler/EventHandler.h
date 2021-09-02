#pragma once

#include "../../Subsystem.h"
#include "../../Event.h"

namespace vox
{
	class EventHandler :
		public Subsystem
	{
	public:
		EventHandler() = default;
		void onAttach() override;

		template<class T>
		void subscribeEvent(const std::function<void(const T&)>& callback);

		template<class T> void sendEvent(const T& event);
	private:
		void* getCallbackList(EventType type);

		std::vector<std::function<void(const KeyEvent&)>> m_KeyEventCallbacks;
		std::vector<std::function<void(const MouseMoveEvent&)>> m_MouseMoveEventCallbacks;
		std::vector<std::function<void(const MouseClickEvent&)>> m_MouseClickEventCallbacks;
	};

	template<class T>
	void EventHandler::subscribeEvent(const std::function<void(const T&)>& callback)
	{
		auto callbackVector = static_cast<std::vector<std::function<void(const T&)>>*>(getCallbackList(T::getStaticType()));
		callbackVector->push_back(callback);
	}

	template<class T>
	void EventHandler::sendEvent(const T& event)
	{
		auto callbackVector = static_cast<std::vector<std::function<void(const T&)>>*>(getCallbackList(T::getStaticType()));
		for (auto& f : *callbackVector)
		{
			f(event);
		}
	}
}


