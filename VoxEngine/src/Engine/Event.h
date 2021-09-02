/*
* This header defines all possible events
 */

#pragma once

#define EVENT_TYPE(x)	static EventType getStaticType() {return EventType::x;}\
						EventType getType() const override {return getStaticType();};\
						static std::string getName() {return #x;}

#define EVENT_CALLBACK(x) [this](const auto& event){this->x(event);}
#define EVENT_CALLBACKK(x) std::bind(&x, this, std::placeholders::_1)

namespace vox
{
	/*
	* When adding new events, also need to add callback list to EventHandler
	*/
	enum class EventType
	{
		Key			= 0,
		MouseMove	= 1,
		MouseClick	= 2,
	};

	class Event
	{
	public:
		virtual EventType getType() const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// Keyboard event ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class KeyEvent : public Event
	{
	public:
		KeyEvent() = default;
		KeyEvent(int key);
		KeyEvent(int key, int action);

		int getKey() const;
		int getAction() const;

		EVENT_TYPE(Key)
	private:
		int m_Key;
		int m_Action;
	};

	//////////////////////////////////////////////////////////////////////////
	// Mouse Move Event //////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent() = default;
		MouseMoveEvent(int dx, int dy);

		int getDx() const;
		int getDy() const;

		EVENT_TYPE(MouseMove)
	private:
		int m_Dx;
		int m_Dy;
	};

	class MouseClickEvent : public Event
	{};
}

