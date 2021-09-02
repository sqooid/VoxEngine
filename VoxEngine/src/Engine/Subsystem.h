#pragma once

namespace vox
{
	class Renderer;
	class EventHandler;

	class Archsystem;

	enum class SubType
	{
		Renderer,
		EventHandler,
	};

	class Subsystem
	{
	public:
		Subsystem();
		Subsystem(Archsystem* parent);

		void setParent(Archsystem* parent);

		virtual void onAttach() {};

		/**
		 * @brief Contains all things that need to be run by subsystem before each frame is rendered
		*/
		virtual void onUpdate() {};

		/**
		 * @brief Gives pointer to another subsystem contained in over-arching system
		 * @param type Enumerated type of system to get pointer to
		 * @return Pointer to specified subsystem
		*/
		Subsystem* getSub(SubType type);
		Renderer* getRenderer();
		EventHandler* getEventHandler();
	private:
		Archsystem* m_Parent;
	};
}

